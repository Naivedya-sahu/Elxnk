#!/bin/bash
# Simple wrapper to draw text
# Usage: draw_text.sh <text> <x> <y> [scale]

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN_DIR="$SCRIPT_DIR/../bin"

if [ $# -lt 3 ]; then
    echo "Usage: draw_text.sh <text> <x> <y> [scale]" >&2
    echo "" >&2
    echo "Example: draw_text.sh 'Hello RM2' 300 200 1.5" >&2
    echo "" >&2
    echo "Available characters:" >&2
    "$BIN_DIR/render_json.py" list 2>/dev/null | grep -A 2 "Available Font" | tail -1 || true
    exit 1
fi

TEXT="$1"
X="$2"
Y="$3"
SCALE="${4:-1.0}"

# Validate numeric arguments
if ! [[ "$X" =~ ^[0-9]+$ ]]; then
    echo "Error: X coordinate must be a number, got: $X" >&2
    exit 1
fi

if ! [[ "$Y" =~ ^[0-9]+$ ]]; then
    echo "Error: Y coordinate must be a number, got: $Y" >&2
    exit 1
fi

if ! [[ "$SCALE" =~ ^[0-9]+\.?[0-9]*$ ]]; then
    echo "Error: Scale must be a number, got: $SCALE" >&2
    exit 1
fi

# Execute draw command
if ! "$BIN_DIR/draw_remote" text "$TEXT" "$X" "$Y" "$SCALE"; then
    echo "Error: Failed to draw text '$TEXT'" >&2
    exit 1
fi
