# Elnx-Simple - JSON-Based Circuit Drawing for reMarkable 2

Simple, WiFi-enabled circuit drawing system using JSON component library and bash commands.

## Architecture

```
┌─────────────────────────────────────────────────────┐
│ Host Computer (Linux/Mac)                          │
│  ┌─────────────┐    ┌──────────────┐              │
│  │   JSON      │ -> │ render_json  │ -> Commands  │
│  │ Components  │    │   .py        │              │
│  └─────────────┘    └──────────────┘              │
│         │                  │                        │
│         v                  v                        │
│  ┌─────────────────────────────────┐               │
│  │   draw_remote (WiFi sender)     │               │
│  └─────────────────────────────────┘               │
└─────────────────│────────────────────────────────┬──┘
                  │ SSH over WiFi                  │
                  v                                v
┌─────────────────────────────────────────────────────┐
│ reMarkable 2 Device                                 │
│  ┌─────────────────────────────────┐               │
│  │   lamp (drawing engine)         │               │
│  │   - Geometry functions          │               │
│  │   - Pen/Eraser support          │               │
│  │   - Direct framebuffer access   │               │
│  └─────────────────────────────────┘               │
└─────────────────────────────────────────────────────┘
```

## Quick Start

### 1. Build and Deploy Lamp

```bash
cd Elnx-simple/lamp
make
make deploy HOST=10.11.99.1
```

### 2. Draw Components Over WiFi

```bash
# Set reMarkable 2 IP (optional, default: 10.11.99.1)
export RM2_HOST=10.11.99.1

# Draw a resistor
./scripts/draw_component.sh R 500 400 1.5

# Draw text
./scripts/draw_text.sh "Hello RM2" 300 200 1.0

# Draw complete circuit
./scripts/draw_circuit.sh
```

## Features

### ✅ JSON Component Library
- **Relative coordinates** (0.0-1.0) for easy scaling
- **Bounding box** information
- **Terminal positions** for auto-wiring
- Easy to edit and extend

### ✅ WiFi Command Sender
- Draw from any computer on the network
- No need to deploy apps to reMarkable
- Real-time drawing via SSH

### ✅ Geometry Functions (from rmkit)
- `pen circle` - Draw circles
- `pen line` - Draw lines
- `pen rectangle` - Draw rectangles
- `pen arc` - Draw arcs
- `pen bezier` - Draw bezier curves
- `eraser` - Erase mode

### ✅ Simple Bash Scripts
- `draw_component.sh` - Draw any component
- `draw_text.sh` - Render text
- `draw_circuit.sh` - Example circuit
- `clear_area.sh` - Clear screen area

## Directory Structure

```
Elnx-simple/
├── bin/
│   ├── render_json.py    # JSON to lamp command converter
│   └── draw_remote        # WiFi command sender
├── lib/
│   ├── components.json   # Component library (relative coords)
│   └── fonts.json        # Font library (relative coords)
├── scripts/
│   ├── draw_component.sh # Draw single component
│   ├── draw_text.sh      # Draw text
│   ├── draw_circuit.sh   # Example circuit
│   └── clear_area.sh     # Clear screen area
├── lamp/
│   ├── main.cpp          # Lamp source with geometry functions
│   └── Makefile          # Build for reMarkable 2
└── README.md            # This file
```

## Usage Examples

### List Available Components

```bash
./bin/render_json.py list
```

Output:
```
Available Components:
  C          - Capacitor            (passive     ) [22x78px]
  D          - Diode                (semiconductor) [77x28px]
  GND        - Ground               (symbol      ) [26x27px]
  NMOS       - N-Channel MOSFET     (transistor  ) [85x78px]
  R          - Resistor             (passive     ) [22x78px]

Available Font Characters:
  0, 1, A, B
```

### Draw Single Component

```bash
# Syntax: draw_component.sh <name> <x> <y> [scale]

# Draw resistor at center, normal size
./scripts/draw_component.sh R 700 900 1.0

# Draw large capacitor
./scripts/draw_component.sh C 500 500 2.0

# Draw MOSFET
./scripts/draw_component.sh NMOS 300 400 1.5
```

### Draw Text

```bash
# Syntax: draw_text.sh <text> <x> <y> [scale]

./scripts/draw_text.sh "R1" 750 900 1.0
./scripts/draw_text.sh "10K" 750 950 0.8
```

### Generate Commands Locally (No WiFi)

```bash
# Generate lamp commands without sending
./bin/render_json.py component R 500 400 1.0

# Output:
# pen down 511 416
# pen move 511 413
# pen move 521 420
# ...
# pen up
```

### Pipe Commands Manually

```bash
# Generate and pipe to lamp
./bin/render_json.py component R 500 400 1.0 | \
  ssh root@10.11.99.1 /opt/bin/lamp
```

### Clear Screen Area

```bash
# Clear rectangle: x, y, width, height
./scripts/clear_area.sh 100 100 800 600
```

### Custom Drawing Script

```bash
#!/bin/bash
# my_circuit.sh - Draw custom circuit

DRAW=./scripts/draw_component.sh
TEXT=./scripts/draw_text.sh

# Draw voltage divider
$DRAW VDC 200 300 1.0  # Voltage source
$DRAW R 400 300 1.0    # R1
$DRAW R 400 500 1.0    # R2
$DRAW GND 400 700 1.0  # Ground

# Label components
$TEXT "V1" 150 300 0.8
$TEXT "R1" 450 300 0.8
$TEXT "R2" 450 500 0.8
```

## JSON Library Format

### Component Definition

```json
{
  "R": {
    "name": "Resistor",
    "type": "passive",
    "bbox": {
      "width": 22,
      "height": 78,
      "unit": "px"
    },
    "terminals": {
      "top": {"x": 0.5, "y": 0.0},
      "bottom": {"x": 0.5, "y": 1.0}
    },
    "paths": [
      {
        "type": "path",
        "commands": [
          {"cmd": "M", "x": 0.5, "y": 0.17},
          {"cmd": "L", "x": 0.95, "y": 0.26},
          ...
        ]
      }
    ]
  }
}
```

### Coordinate System

- **Relative coordinates**: 0.0 to 1.0 (normalized to bounding box)
- **Scaling**: Multiply by desired scale factor
- **Positioning**: Add offset (x, y) to place component
- **Terminal positions**: For auto-wiring (future feature)

## Lamp Commands Reference

### Drawing Commands
```
pen down X Y           # Start drawing at position
pen move X Y           # Draw line to position
pen up                 # Stop drawing
pen line X1 Y1 X2 Y2   # Draw straight line
pen circle CX CY R1 R2 # Draw circle
pen rectangle X1 Y1 X2 Y2   # Draw rectangle
pen arc CX CY R1 R2 A1 A2   # Draw arc (angles in degrees)
pen bezier X1 Y1 ...        # Draw bezier curve
```

### Eraser Commands
```
eraser down X Y        # Start erasing
eraser move X Y        # Erase to position
eraser up              # Stop erasing
```

### Speed Commands
```
fastpen down/move/up   # Faster drawing (2ms vs 10ms)
```

## reMarkable 2 Screen

- **Resolution**: 1404 x 1872 pixels
- **DPI**: ~226 DPI (~8.9 pixels/mm)
- **Coordinate origin**: Top-left (0, 0)
- **Center**: (702, 936)

## Development Workflow

### 1. Edit JSON Library

```bash
# Edit components
vim lib/components.json

# Add new component
# {
#   "LED": {
#     "name": "LED",
#     "bbox": {"width": 30, "height": 40},
#     "paths": [...]
#   }
# }
```

### 2. Test Locally

```bash
# Generate commands
./bin/render_json.py component LED 500 500 1.0

# Verify output
```

### 3. Draw on reMarkable

```bash
# Draw via WiFi
./scripts/draw_component.sh LED 500 500 1.0
```

## Extending the Library

### Add New Component

1. Edit `lib/components.json`
2. Define bounding box
3. Add paths with relative coordinates
4. Test: `./bin/render_json.py component NEWCOMP 500 500`

### Add New Font Character

1. Edit `lib/fonts.json`
2. Define character paths
3. Test: `./bin/render_json.py text "ABC" 500 500`

## Advantages Over Embedded C++ Approach

| Feature | Elnx-Simple (JSON) | Elxnk (C++) |
|---------|-------------------|-------------|
| **Edit components** | Edit JSON, instant | Rebuild C++, redeploy |
| **Drawing method** | WiFi from any PC | On-device only |
| **Add components** | Add JSON entry | Regenerate header, rebuild |
| **Library format** | Human-readable JSON | Binary embedded C++ |
| **Deploy** | Once (lamp only) | Full rebuild each time |
| **Test** | Instant over WiFi | Deploy + run on device |
| **Coordinates** | Relative (scalable) | Absolute pixels |

## Troubleshooting

### Cannot connect to reMarkable

```bash
# Check IP address
ping 10.11.99.1

# Test SSH
ssh root@10.11.99.1 true

# Set correct IP
export RM2_HOST=192.168.1.xxx
```

### Lamp not found

```bash
# Deploy lamp
cd lamp
make deploy
```

### Component not rendering

```bash
# Test command generation
./bin/render_json.py component R 500 500

# Check JSON syntax
python3 -m json.tool lib/components.json
```

### Drawing appears in wrong location

- reMarkable 2 uses top-left origin (0, 0)
- Center is at (702, 936)
- Check X coordinate < 1404, Y coordinate < 1872

## Future Enhancements

- [ ] Auto-wiring between component terminals
- [ ] Circuit schematic templates
- [ ] Component rotation
- [ ] Color support (for reMarkable Color)
- [ ] Undo/redo via command history
- [ ] Component library browser
- [ ] JSON schema validation
- [ ] Batch drawing from circuit definition files

## License

Based on rmkit lamp implementation with geometry functions.

## Credits

- **rmkit** - Original lamp and iago implementations
- **reMarkable 2** - E-ink tablet platform
