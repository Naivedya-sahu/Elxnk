#!/bin/bash
# Clear a rectangular area on the screen using eraser
# Usage: clear_area.sh <x> <y> <width> <height>

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN_DIR="$SCRIPT_DIR/../bin"
HOST="${RM2_HOST:-10.11.99.1}"

if [ $# -lt 4 ]; then
    echo "Usage: clear_area.sh <x> <y> <width> <height>"
    echo "Example: clear_area.sh 100 100 800 600"
    exit 1
fi

X="$1"
Y="$2"
WIDTH="$3"
HEIGHT="$4"

# Generate eraser rectangle commands
COMMANDS="eraser down $X $Y
eraser move $((X + WIDTH)) $Y
eraser move $((X + WIDTH)) $((Y + HEIGHT))
eraser move $X $((Y + HEIGHT))
eraser move $X $Y
eraser up"

echo "$COMMANDS" | ssh root@"$HOST" /opt/bin/lamp

echo "✓ Cleared area at ($X, $Y) size ${WIDTH}x${HEIGHT}"
