#!/bin/bash
# Simple wrapper to draw circuit components
# Usage: draw_component.sh <component_name> <x> <y> [scale]

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN_DIR="$SCRIPT_DIR/../bin"

if [ $# -lt 3 ]; then
    echo "Usage: draw_component.sh <component_name> <x> <y> [scale]"
    echo ""
    echo "Available components:"
    "$BIN_DIR/render_json.py" list | grep -A 100 "Available Components:" | grep -v "Available"
    exit 1
fi

COMPONENT="$1"
X="$2"
Y="$3"
SCALE="${4:-1.0}"

"$BIN_DIR/draw_remote" component "$COMPONENT" "$X" "$Y" "$SCALE"
