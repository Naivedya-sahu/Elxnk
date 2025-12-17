#!/bin/bash
# Font rendering test script
# Renders text using individual character commands

LAMP_PIPE="/tmp/elxnk_lamp.pipe"

# Check if lamp pipe exists
if [ ! -p "$LAMP_PIPE" ]; then
    echo "Error: Lamp pipe not found at $LAMP_PIPE"
    echo "Is elxnk running?"
    exit 1
fi

# Render letter 'A' at position with scale
render_A() {
    local x=$1
    local y=$2
    local scale=${3:-1.0}

    # Simple A shape (triangle with crossbar)
    {
        echo "pen up"
        echo "pen move $x $((y + 40))"
        echo "pen down"
        echo "pen move $((x + 20)) $y"
        echo "pen move $((x + 40)) $((y + 40))"
        echo "pen up"
        echo "pen move $((x + 10)) $((y + 25))"
        echo "pen down"
        echo "pen move $((x + 30)) $((y + 25))"
        echo "pen up"
    } > "$LAMP_PIPE"
}

# Render letter 'B'
render_B() {
    local x=$1
    local y=$2

    {
        echo "pen up"
        echo "pen move $x $y"
        echo "pen down"
        echo "pen move $x $((y + 40))"
        echo "pen move $((x + 25)) $((y + 40))"
        echo "pen move $((x + 30)) $((y + 35))"
        echo "pen move $((x + 30)) $((y + 25))"
        echo "pen move $((x + 25)) $((y + 20))"
        echo "pen move $x $((y + 20))"
        echo "pen up"
        echo "pen move $((x + 25)) $((y + 20))"
        echo "pen down"
        echo "pen move $((x + 30)) $((y + 15))"
        echo "pen move $((x + 30)) $((y + 5))"
        echo "pen move $((x + 25)) $y"
        echo "pen move $x $y"
        echo "pen up"
    } > "$LAMP_PIPE"
}

# Render letter 'C'
render_C() {
    local x=$1
    local y=$2

    {
        echo "pen up"
        echo "pen move $((x + 30)) $y"
        echo "pen down"
        echo "pen move $x $y"
        echo "pen move $x $((y + 40))"
        echo "pen move $((x + 30)) $((y + 40))"
        echo "pen up"
    } > "$LAMP_PIPE"
}

# Render digit '0'
render_0() {
    local x=$1
    local y=$2

    {
        echo "pen up"
        echo "pen move $((x + 10)) $y"
        echo "pen down"
        echo "pen move $((x + 20)) $y"
        echo "pen move $((x + 30)) $((y + 10))"
        echo "pen move $((x + 30)) $((y + 30))"
        echo "pen move $((x + 20)) $((y + 40))"
        echo "pen move $((x + 10)) $((y + 40))"
        echo "pen move $x $((y + 30))"
        echo "pen move $x $((y + 10))"
        echo "pen move $((x + 10)) $y"
        echo "pen up"
    } > "$LAMP_PIPE"
}

# Render digit '1'
render_1() {
    local x=$1
    local y=$2

    {
        echo "pen up"
        echo "pen move $((x + 15)) $y"
        echo "pen down"
        echo "pen move $((x + 15)) $((y + 40))"
        echo "pen up"
        echo "pen move $((x + 10)) $((y + 10))"
        echo "pen down"
        echo "pen move $((x + 15)) $y"
        echo "pen up"
    } > "$LAMP_PIPE"
}

# Render text string
render_text() {
    local text="$1"
    local x=$2
    local y=$3
    local spacing=50

    echo "Rendering: '$text' at ($x, $y)"

    for (( i=0; i<${#text}; i++ )); do
        char="${text:$i:1}"
        case "$char" in
            A) render_A $x $y ;;
            B) render_B $x $y ;;
            C) render_C $x $y ;;
            0) render_0 $x $y ;;
            1) render_1 $x $y ;;
            ' ') : ;;  # Space - just advance
            *) echo "Unknown char: $char" ;;
        esac
        x=$((x + spacing))
    done
}

# Clear area with eraser
clear_area() {
    local x1=$1
    local y1=$2
    local x2=$3
    local y2=$4

    {
        echo "eraser on"
        echo "pen down $x1 $y1"
        echo "pen move $x2 $y1"
        echo "pen move $x2 $y2"
        echo "pen move $x1 $y2"
        echo "pen move $x1 $y1"
        echo "pen up"
        echo "eraser off"
    } > "$LAMP_PIPE"
}

# Main test
echo "=== Font Rendering Test ==="
echo "Pipe: $LAMP_PIPE"
echo ""

# Test 1: Individual letters
echo "[1/4] Testing individual letters..."
render_A 100 100
sleep 1
render_B 200 100
sleep 1
render_C 300 100
sleep 1

# Test 2: Numbers
echo "[2/4] Testing numbers..."
render_0 100 300
sleep 1
render_1 200 300
sleep 1

# Test 3: Text string
echo "[3/4] Testing text string..."
render_text "ABC" 100 500

# Test 4: Large text
echo "[4/4] Testing larger text..."
sleep 2
render_text "A0B1C" 100 700

echo ""
echo "=== Test Complete ==="
echo "Text should be visible on screen"
