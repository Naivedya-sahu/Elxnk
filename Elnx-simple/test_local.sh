#!/bin/bash
# Local Testing Script - No reMarkable 2 Required
# Tests JSON rendering and command generation

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "========================================="
echo "  Elnx-Simple Local Test Suite"
echo "========================================="
echo ""

# Test 1: List components
echo "[1/6] Testing component list..."
OUTPUT=$(python3 bin/render_json.py list 2>&1)
if echo "$OUTPUT" | grep -q "Available Components"; then
    echo "✓ Component listing works"
    COMPONENT_COUNT=$(echo "$OUTPUT" | grep -c "^\s\+[A-Z]" || true)
    echo "  Found $COMPONENT_COUNT components"
else
    echo "❌ Failed to list components"
    exit 1
fi
echo ""

# Test 2: Render resistor
echo "[2/6] Testing resistor rendering..."
OUTPUT=$(python3 bin/render_json.py component R 500 400 1.0 2>&1)
if echo "$OUTPUT" | grep -q "pen down"; then
    echo "✓ Resistor renders correctly"
    CMD_COUNT=$(echo "$OUTPUT" | wc -l)
    echo "  Generated $CMD_COUNT lamp commands"
else
    echo "❌ Failed to render resistor"
    exit 1
fi
echo ""

# Test 3: Render all components
echo "[3/6] Testing all components..."
COMPONENTS=("R" "C" "D" "GND" "NMOS")
FAILED=()

for comp in "${COMPONENTS[@]}"; do
    if ! python3 bin/render_json.py component "$comp" 500 500 1.0 > /dev/null 2>&1; then
        FAILED+=("$comp")
    fi
done

if [ ${#FAILED[@]} -eq 0 ]; then
    echo "✓ All ${#COMPONENTS[@]} components render correctly"
else
    echo "❌ Failed components: ${FAILED[*]}"
    exit 1
fi
echo ""

# Test 4: Render text
echo "[4/6] Testing text rendering..."
OUTPUT=$(python3 bin/render_json.py text "AB01" 300 200 1.0 2>&1)
if echo "$OUTPUT" | grep -q "pen down"; then
    echo "✓ Text renders correctly"
    CMD_COUNT=$(echo "$OUTPUT" | wc -l)
    echo "  Generated $CMD_COUNT lamp commands for 'AB01'"
else
    echo "❌ Failed to render text"
    exit 1
fi
echo ""

# Test 5: Test scaling
echo "[5/6] Testing component scaling..."
OUTPUT_1X=$(python3 bin/render_json.py component R 500 500 1.0 2>&1)
OUTPUT_2X=$(python3 bin/render_json.py component R 500 500 2.0 2>&1)

if [ "$OUTPUT_1X" != "$OUTPUT_2X" ]; then
    echo "✓ Scaling works (1.0x vs 2.0x produce different output)"
else
    echo "❌ Scaling doesn't work correctly"
    exit 1
fi
echo ""

# Test 6: Test positioning
echo "[6/6] Testing component positioning..."
OUTPUT_POS1=$(python3 bin/render_json.py component R 100 100 1.0 2>&1)
OUTPUT_POS2=$(python3 bin/render_json.py component R 500 500 1.0 2>&1)

if [ "$OUTPUT_POS1" != "$OUTPUT_POS2" ]; then
    echo "✓ Positioning works (different positions produce different output)"
else
    echo "❌ Positioning doesn't work correctly"
    exit 1
fi
echo ""

# Summary
echo "========================================="
echo "  ✅ All Local Tests Passed!"
echo "========================================="
echo ""
echo "Example commands generated:"
echo ""
echo "Component R at (500, 400):"
python3 bin/render_json.py component R 500 400 1.0 | head -5
echo "..."
echo ""
echo "Text 'AB' at (300, 200):"
python3 bin/render_json.py text "AB" 300 200 1.0 | head -5
echo "..."
echo ""
echo "To test on reMarkable 2:"
echo "  1. Run ./setup.sh"
echo "  2. cd lamp && make deploy"
echo "  3. ./scripts/draw_component.sh R 500 400 1.0"
echo ""
