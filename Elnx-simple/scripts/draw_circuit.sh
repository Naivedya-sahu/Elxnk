#!/bin/bash
# Example: Draw a simple circuit with components and labels
# This demonstrates how to build complex drawings from simple commands

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DRAW="$SCRIPT_DIR/draw_component.sh"
TEXT="$SCRIPT_DIR/draw_text.sh"

# Define drawing area
# reMarkable 2 screen: 1404 x 1872 pixels
CENTER_X=702
CENTER_Y=936

echo "Drawing simple RC circuit..."

# Draw circuit components
# Resistor R1 at top
$DRAW R $((CENTER_X - 200)) $((CENTER_Y - 300)) 1.5

# Capacitor C1 at bottom
$DRAW C $((CENTER_X - 200)) $((CENTER_Y - 100)) 1.5

# Ground symbol
$DRAW GND $((CENTER_X - 200)) $((CENTER_Y + 100)) 1.0

# Voltage source
$DRAW VDC $((CENTER_X - 400)) $((CENTER_Y - 100)) 1.0

# Add labels
$TEXT "R1" $((CENTER_X - 150)) $((CENTER_Y - 300)) 0.8
$TEXT "C1" $((CENTER_X - 150)) $((CENTER_Y - 100)) 0.8
$TEXT "V1" $((CENTER_X - 450)) $((CENTER_Y - 100)) 0.8

echo "Circuit drawn successfully!"
