#!/bin/bash
# Build Component Library Header
# Generates component_library.h from all SVG assets using svg_to_lamp_final.py

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"

CONVERTER="$PROJECT_ROOT/tools/svg_to_lamp_final.py"
COMPONENTS_DIR="$PROJECT_ROOT/assets/components"
FONTS_DIR="$PROJECT_ROOT/assets/font"
OUTPUT_HEADER="$PROJECT_ROOT/src/elxnk/component_library.h"

echo "============================================"
echo "Building Component Library Header"
echo "============================================"
echo "Converter: $CONVERTER"
echo "Components: $COMPONENTS_DIR"
echo "Fonts: $FONTS_DIR"
echo "Output: $OUTPUT_HEADER"
echo ""

# Check dependencies
if ! python3 -c "import svgpathtools" 2>/dev/null; then
    echo "ERROR: svgpathtools not installed"
    echo "Install: pip install svgpathtools"
    exit 1
fi

# Check converter exists
if [ ! -f "$CONVERTER" ]; then
    echo "ERROR: Converter not found: $CONVERTER"
    exit 1
fi

# Check directories exist
if [ ! -d "$COMPONENTS_DIR" ]; then
    echo "ERROR: Components directory not found: $COMPONENTS_DIR"
    exit 1
fi

if [ ! -d "$FONTS_DIR" ]; then
    echo "ERROR: Fonts directory not found: $FONTS_DIR"
    exit 1
fi

# Backup old header if it exists
if [ -f "$OUTPUT_HEADER" ]; then
    BACKUP="$OUTPUT_HEADER.backup.$(date +%Y%m%d_%H%M%S)"
    cp "$OUTPUT_HEADER" "$BACKUP"
    echo "✓ Backed up existing header to $(basename $BACKUP)"
fi

# Create output directory if needed
mkdir -p "$(dirname "$OUTPUT_HEADER")"

# Generate new header
echo ""
echo "Generating header file..."
echo "----------------------------------------"
python3 "$CONVERTER" --header "$COMPONENTS_DIR" "$FONTS_DIR" "$OUTPUT_HEADER"

if [ $? -eq 0 ]; then
    echo ""
    echo "============================================"
    echo "✓ SUCCESS: Header file generated"
    echo "============================================"
    echo "  File: $OUTPUT_HEADER"
    echo "  Size: $(wc -c < "$OUTPUT_HEADER") bytes"
    echo "  Lines: $(wc -l < "$OUTPUT_HEADER") lines"
    echo ""
    
    # Count components and fonts
    COMP_COUNT=$(grep -c "^// Component:" "$OUTPUT_HEADER" || true)
    FONT_COUNT=$(grep -c "^// Font:" "$OUTPUT_HEADER" || true)
    CMD_COUNT=$(grep -c '{"pen' "$OUTPUT_HEADER" || true)
    
    echo "  Components: $COMP_COUNT"
    echo "  Fonts: $FONT_COUNT"
    echo "  Total commands: $CMD_COUNT"
    echo ""
    echo "Next steps:"
    echo "  1. Review header: less $OUTPUT_HEADER"
    echo "  2. Build binary: cd src && make clean && make"
    echo "  3. Test locally: cd src && make test"
    echo "  4. Deploy: cd src && make install HOST=10.11.99.1"
    echo ""
else
    echo ""
    echo "============================================"
    echo "✗ FAILED: Header generation failed"
    echo "============================================"
    
    # Restore backup if exists
    if [ -f "$BACKUP" ]; then
        echo "Restoring backup..."
        mv "$BACKUP" "$OUTPUT_HEADER"
        echo "✓ Original header restored"
    fi
    
    exit 1
fi
