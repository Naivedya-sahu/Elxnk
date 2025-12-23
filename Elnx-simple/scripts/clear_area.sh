#!/bin/bash
# Clear a rectangular area on the screen using eraser
# Usage: clear_area.sh <x> <y> <width> <height>

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN_DIR="$SCRIPT_DIR/../bin"
HOST="${RM2_HOST:-10.11.99.1}"

if [ $# -lt 4 ]; then
    echo "Usage: clear_area.sh <x> <y> <width> <height>" >&2
    echo "Example: clear_area.sh 100 100 800 600" >&2
    echo "" >&2
    echo "Clears a rectangular area using eraser mode" >&2
    exit 1
fi

X="$1"
Y="$2"
WIDTH="$3"
HEIGHT="$4"

# Validate numeric arguments
for arg in "$X" "$Y" "$WIDTH" "$HEIGHT"; do
    if ! [[ "$arg" =~ ^[0-9]+$ ]]; then
        echo "Error: All arguments must be numbers" >&2
        exit 1
    fi
done

# Check SSH
if ! command -v ssh >/dev/null 2>&1; then
    echo "Error: SSH client not found" >&2
    exit 1
fi

# Generate eraser rectangle commands
COMMANDS="eraser down $X $Y
eraser move $((X + WIDTH)) $Y
eraser move $((X + WIDTH)) $((Y + HEIGHT))
eraser move $X $((Y + HEIGHT))
eraser move $X $Y
eraser up"

# Send commands
if echo "$COMMANDS" | ssh -o ConnectTimeout=2 root@"$HOST" /opt/bin/lamp 2>/dev/null; then
    echo "✓ Cleared area at ($X, $Y) size ${WIDTH}x${HEIGHT}" >&2
else
    echo "Error: Failed to connect to reMarkable 2 at $HOST" >&2
    echo "Set RM2_HOST environment variable with correct IP" >&2
    exit 1
fi
