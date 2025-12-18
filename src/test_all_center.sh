#!/bin/bash
# Test script to render all components and fonts at screen center

LAMP_PIPE="/tmp/elxnk_lamp.pipe"
CENTER_X=702  # 1404/2
CENTER_Y=936  # 1872/2

echo "=== Testing All Components at Screen Center ==="

# Test components
echo "Testing: R (Resistor)"
render_component R $CENTER_X $CENTER_Y 1.0

sleep 1

echo "Testing: NMOS (Transistor)"
render_component NMOS $CENTER_X $CENTER_Y 1.0

sleep 1

echo "Testing: D (Diode)"
render_component D $CENTER_X $CENTER_Y 1.0

sleep 1

echo "Testing: VDC (Voltage Source)"
render_component VDC $CENTER_X $CENTER_Y 1.0

sleep 1

# Test fonts
echo "Testing: Font rendering"
render_component text $CENTER_X $CENTER_Y "HELLO123"

echo "=== Test Complete ==="
