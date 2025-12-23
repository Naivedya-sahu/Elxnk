#!/bin/bash
# Example: Draw a simple circuit with components and labels
# This demonstrates how to build complex drawings from simple commands

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DRAW="$SCRIPT_DIR/draw_component.sh"
TEXT="$SCRIPT_DIR/draw_text.sh"

# Define drawing area
# reMarkable 2 screen: 1404 x 1872 pixels
CENTER_X=702
CENTER_Y=936

echo "Drawing simple RC circuit..." >&2

# Draw circuit components
echo "  Drawing R1 resistor..." >&2
$DRAW R $((CENTER_X - 200)) $((CENTER_Y - 300)) 1.5

echo "  Drawing C1 capacitor..." >&2
$DRAW C $((CENTER_X - 200)) $((CENTER_Y - 100)) 1.5

echo "  Drawing ground..." >&2
$DRAW GND $((CENTER_X - 200)) $((CENTER_Y + 100)) 1.0

# Add labels
echo "  Adding labels..." >&2
$TEXT "R1" $((CENTER_X - 150)) $((CENTER_Y - 300)) 0.8
$TEXT "C1" $((CENTER_X - 150)) $((CENTER_Y - 100)) 0.8

echo "✓ Circuit drawn successfully!" >&2
