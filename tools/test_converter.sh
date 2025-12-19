#!/bin/bash
# Test svg_to_lamp_final.py on all SVG files
# Shows detailed statistics and identifies problematic files

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR/.."

CONVERTER="$PROJECT_ROOT/tools/svg_to_lamp_final.py"
COMPONENTS_DIR="$PROJECT_ROOT/assets/components"
FONTS_DIR="$PROJECT_ROOT/assets/font"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "============================================"
echo "SVG to LAMP Converter Test Suite"
echo "============================================"
echo ""

# Check dependencies
if ! python3 -c "import svgpathtools" 2>/dev/null; then
    echo -e "${RED}ERROR: svgpathtools not installed${NC}"
    echo "Install: pip install svgpathtools"
    exit 1
fi

if [ ! -f "$CONVERTER" ]; then
    echo -e "${RED}ERROR: Converter not found: $CONVERTER${NC}"
    exit 1
fi

# Test function
test_svg() {
    local svg_file="$1"
    local name=$(basename "$svg_file" .svg)
    
    # Run converter and capture output
    if output=$(python3 "$CONVERTER" "$svg_file" 2>&1); then
        # Count commands
        cmd_count=$(echo "$output" | grep -c "^pen " || true)
        
        if [ "$cmd_count" -gt 0 ]; then
            echo -e "${GREEN}✓${NC} $name: $cmd_count commands"
            return 0
        else
            echo -e "${RED}✗${NC} $name: No commands generated"
            return 1
        fi
    else
        echo -e "${RED}✗${NC} $name: Parse error"
        echo "$output" | head -3
        return 1
    fi
}

# Test components
echo "Testing Components..."
echo "--------------------"
comp_pass=0
comp_fail=0
comp_total=0

if [ -d "$COMPONENTS_DIR" ]; then
    for svg in "$COMPONENTS_DIR"/*.svg; do
        [ -f "$svg" ] || continue
        [ "$(basename "$svg")" = "Library.svg" ] && continue
        
        comp_total=$((comp_total + 1))
        if test_svg "$svg"; then
            comp_pass=$((comp_pass + 1))
        else
            comp_fail=$((comp_fail + 1))
        fi
    done
fi

echo ""
echo "Testing Fonts..."
echo "----------------"
font_pass=0
font_fail=0
font_total=0

if [ -d "$FONTS_DIR" ]; then
    for svg in "$FONTS_DIR"/*.svg; do
        [ -f "$svg" ] || continue
        
        font_total=$((font_total + 1))
        if test_svg "$svg"; then
            font_pass=$((font_pass + 1))
        else
            font_fail=$((font_fail + 1))
        fi
    done
fi

# Summary
echo ""
echo "============================================"
echo "Test Summary"
echo "============================================"
echo "Components:"
echo "  Total: $comp_total"
echo -e "  Passed: ${GREEN}$comp_pass${NC}"
[ "$comp_fail" -gt 0 ] && echo -e "  Failed: ${RED}$comp_fail${NC}" || echo -e "  Failed: ${GREEN}0${NC}"

echo ""
echo "Fonts:"
echo "  Total: $font_total"
echo -e "  Passed: ${GREEN}$font_pass${NC}"
[ "$font_fail" -gt 0 ] && echo -e "  Failed: ${RED}$font_fail${NC}" || echo -e "  Failed: ${GREEN}0${NC}"

echo ""
echo "Overall:"
total=$((comp_total + font_total))
pass=$((comp_pass + font_pass))
fail=$((comp_fail + font_fail))
success_rate=$(awk "BEGIN {printf \"%.1f\", ($pass/$total)*100}")

echo "  Total files: $total"
echo -e "  Success rate: ${GREEN}$success_rate%${NC}"

if [ "$fail" -eq 0 ]; then
    echo ""
    echo -e "${GREEN}✓ ALL TESTS PASSED${NC}"
    echo ""
    echo "Ready to generate header:"
    echo "  ./rebuild_library.sh"
    exit 0
else
    echo ""
    echo -e "${YELLOW}⚠ SOME TESTS FAILED${NC}"
    echo "Fix failing SVGs before generating header"
    exit 1
fi
