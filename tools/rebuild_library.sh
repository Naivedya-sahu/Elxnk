#!/bin/bash
# Regenerate component_library.h with new converter
# Usage: ./rebuild_library.sh

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR/.."

CONVERTER="$PROJECT_ROOT/tools/svg_to_lamp_final.py"
COMPONENTS_DIR="$PROJECT_ROOT/assets/components"
FONTS_DIR="$PROJECT_ROOT/assets/font"
OUTPUT_HEADER="$PROJECT_ROOT/src/elxnk/component_library.h"

echo "============================================"
echo "Rebuilding Component Library"
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

# Backup old header
if [ -f "$OUTPUT_HEADER" ]; then
    cp "$OUTPUT_HEADER" "$OUTPUT_HEADER.backup"
    echo "Backed up existing header to $OUTPUT_HEADER.backup"
fi

# Generate new header
echo ""
echo "Generating header file..."
python3 "$CONVERTER" --header "$COMPONENTS_DIR" "$FONTS_DIR" "$OUTPUT_HEADER"

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ SUCCESS: Header file generated"
    echo "  File: $OUTPUT_HEADER"
    echo "  Size: $(wc -c < "$OUTPUT_HEADER") bytes"
    echo ""
    echo "Next steps:"
    echo "  1. Inspect header: less $OUTPUT_HEADER"
    echo "  2. Rebuild binary: cd src && make"
    echo "  3. Test locally: python3 $CONVERTER assets/components/R.svg"
else
    echo ""
    echo "✗ FAILED: Header generation failed"
    if [ -f "$OUTPUT_HEADER.backup" ]; then
        echo "Restoring backup..."
        mv "$OUTPUT_HEADER.backup" "$OUTPUT_HEADER"
    fi
    exit 1
fi
