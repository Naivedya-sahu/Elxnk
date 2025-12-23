#!/bin/bash
# Simple wrapper to draw circuit components
# Usage: draw_component.sh <component_name> <x> <y> [scale]

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN_DIR="$SCRIPT_DIR/../bin"

if [ $# -lt 3 ]; then
    echo "Usage: draw_component.sh <component_name> <x> <y> [scale]" >&2
    echo "" >&2
    echo "Example: draw_component.sh R 500 400 1.5" >&2
    echo "" >&2
    echo "Available components:" >&2
    "$BIN_DIR/render_json.py" list 2>/dev/null | grep -A 100 "Available Components:" | grep -v "Available" || true
    exit 1
fi

COMPONENT="$1"
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
if ! "$BIN_DIR/draw_remote" component "$COMPONENT" "$X" "$Y" "$SCALE"; then
    echo "Error: Failed to draw component $COMPONENT" >&2
    exit 1
fi
