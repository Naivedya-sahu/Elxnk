#!/bin/bash
# Component rendering test script
# Sends lamp commands directly to test component drawing

LAMP_PIPE="/tmp/elxnk_lamp.pipe"

# Check if lamp pipe exists
if [ ! -p "$LAMP_PIPE" ]; then
    echo "Error: Lamp pipe not found at $LAMP_PIPE"
    echo "Is elxnk running?"
    exit 1
fi

# Component: R (Resistor) - scaled and positioned
render_resistor() {
    local x=$1
    local y=$2
    local scale=${3:-1.0}

    echo "Drawing Resistor at ($x, $y) scale=$scale"

    # Send commands to lamp pipe
    {
        echo "pen up"
        echo "pen move $x $y"
        echo "pen down"
        echo "pen move $((x + 50)) $y"
        echo "pen move $((x + 60)) $((y + 10))"
        echo "pen move $((x + 80)) $((y - 10))"
        echo "pen move $((x + 100)) $((y + 10))"
        echo "pen move $((x + 120)) $((y - 10))"
        echo "pen move $((x + 130)) $y"
        echo "pen move $((x + 180)) $y"
        echo "pen up"
    } > "$LAMP_PIPE"
}

# Component: C (Capacitor)
render_capacitor() {
    local x=$1
    local y=$2

    echo "Drawing Capacitor at ($x, $y)"

    {
        echo "pen up"
        echo "pen move $x $y"
        echo "pen down"
        echo "pen move $((x + 60)) $y"
        echo "pen up"
        echo "pen move $((x + 60)) $((y - 30))"
        echo "pen down"
        echo "pen move $((x + 60)) $((y + 30))"
        echo "pen up"
        echo "pen move $((x + 70)) $((y - 30))"
        echo "pen down"
        echo "pen move $((x + 70)) $((y + 30))"
        echo "pen up"
        echo "pen move $((x + 70)) $y"
        echo "pen down"
        echo "pen move $((x + 130)) $y"
        echo "pen up"
    } > "$LAMP_PIPE"
}

# Component: GND (Ground)
render_ground() {
    local x=$1
    local y=$2

    echo "Drawing Ground at ($x, $y)"

    {
        echo "pen up"
        echo "pen move $x $y"
        echo "pen down"
        echo "pen move $x $((y + 40))"
        echo "pen up"
        echo "pen move $((x - 30)) $((y + 40))"
        echo "pen down"
        echo "pen move $((x + 30)) $((y + 40))"
        echo "pen up"
        echo "pen move $((x - 20)) $((y + 50))"
        echo "pen down"
        echo "pen move $((x + 20)) $((y + 50))"
        echo "pen up"
        echo "pen move $((x - 10)) $((y + 60))"
        echo "pen down"
        echo "pen move $((x + 10)) $((y + 60))"
        echo "pen up"
    } > "$LAMP_PIPE"
}

# Clear screen with eraser
clear_screen() {
    echo "Clearing screen..."
    {
        echo "eraser on"
        echo "pen down 0 0"
        echo "pen move 1404 0"
        echo "pen move 1404 1872"
        echo "pen move 0 1872"
        echo "pen move 0 0"
        echo "pen up"
        echo "eraser off"
    } > "$LAMP_PIPE"
    sleep 1
}

# Main test
echo "=== Component Rendering Test ==="
echo "Pipe: $LAMP_PIPE"
echo ""

# Test 1: Draw resistor
echo "[1/6] Testing Resistor..."
render_resistor 100 100
sleep 2

# Test 2: Draw capacitor
echo "[2/6] Testing Capacitor..."
render_capacitor 100 300
sleep 2

# Test 3: Draw ground
echo "[3/6] Testing Ground..."
render_ground 100 500
sleep 2

# Test 4: Draw multiple components
echo "[4/6] Testing multiple components..."
render_resistor 400 100
render_capacitor 400 300
render_ground 400 500
sleep 2

# Test 5: Test erase mode
echo "[5/6] Testing erase mode..."
{
    echo "eraser on"
    echo "pen down 100 100"
    echo "pen move 300 100"
    echo "pen move 300 200"
    echo "pen move 100 200"
    echo "pen move 100 100"
    echo "pen up"
    echo "eraser off"
} > "$LAMP_PIPE"
sleep 2

# Test 6: Test pen/eraser toggle
echo "[6/6] Testing pen/eraser toggle..."
{
    echo "pen down 700 700"
    echo "pen move 900 700"
    echo "pen move 900 900"
    echo "pen up"
    echo "eraser on"
    echo "pen down 700 750"
    echo "pen move 900 750"
    echo "pen up"
    echo "eraser off"
} > "$LAMP_PIPE"

echo ""
echo "=== Test Complete ==="
echo "Components should be visible on screen"
