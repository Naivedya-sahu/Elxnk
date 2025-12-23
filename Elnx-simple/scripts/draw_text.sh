#!/bin/bash
# Simple wrapper to draw text
# Usage: draw_text.sh <text> <x> <y> [scale]

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN_DIR="$SCRIPT_DIR/../bin"

if [ $# -lt 3 ]; then
    echo "Usage: draw_text.sh <text> <x> <y> [scale]"
    echo ""
    echo "Example: draw_text.sh 'Hello RM2' 300 200 1.5"
    exit 1
fi

TEXT="$1"
X="$2"
Y="$3"
SCALE="${4:-1.0}"

"$BIN_DIR/draw_remote" text "$TEXT" "$X" "$Y" "$SCALE"
