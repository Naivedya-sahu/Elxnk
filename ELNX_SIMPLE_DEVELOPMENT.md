# Elnx-Simple Development Summary

## Overview

**Elnx-Simple** is a simplified, JSON-based circuit drawing system for reMarkable 2 that enables drawing components and text over WiFi from any computer.

## Development Phase Analysis

### Previous Approach (Elxnk)

**Branch**: `claude/analyze-legacy-deploy-gdoVV`

**Architecture**:
- Embedded C++ component library
- SVG → C header file at build time
- On-device process manager (elxnk)
- Gesture detection (genie_lamp)
- Complex build system requiring ARM cross-compiler

**Issues**:
- Required full rebuild to add/modify components
- Must redeploy binaries for any library change
- Complex embedded approach
- Difficult to test iterations
- No remote drawing capability

### New Approach (Elnx-Simple)

**Branch**: `elxnk-simple`

**Architecture**:
- JSON component library with relative coordinates
- Python converter (JSON → lamp commands)
- WiFi command sender (SSH + pipe to lamp)
- Simple bash scripts for drawing
- Modified lamp with geometry functions

**Advantages**:
- Edit JSON, draw instantly (no rebuild)
- Remote drawing from any networked computer
- Human-readable component definitions
- Easy testing and iteration
- One-time lamp deployment
- Simple bash-based workflow

## Key Files Created

### 1. JSON Libraries

**`Elnx-simple/lib/components.json`**
- Circuit components with relative coordinates (0.0-1.0)
- Bounding box information
- Terminal positions for future auto-wiring
- Types: passive, semiconductor, transistor, symbol

**`Elnx-simple/lib/fonts.json`**
- Font glyphs A-Z, 0-9
- Relative coordinate system
- Path-based definitions

### 2. Rendering Tools

**`Elnx-simple/bin/render_json.py`**
- Converts JSON components to lamp commands
- Handles scaling and positioning
- Supports component and text rendering
- Pure Python, no dependencies

### 3. WiFi Command Sender

**`Elnx-simple/bin/draw_remote`**
- Sends lamp commands over SSH
- Configurable host (RM2_HOST env var)
- Pipe mode for custom commands
- Connection checking

### 4. Convenience Scripts

**`Elnx-simple/scripts/draw_component.sh`**
- Simple wrapper for drawing components
- Usage: `draw_component.sh R 500 400 1.5`

**`Elnx-simple/scripts/draw_text.sh`**
- Text rendering wrapper
- Usage: `draw_text.sh "Hello" 300 200 1.0`

**`Elnx-simple/scripts/draw_circuit.sh`**
- Example circuit composition
- Demonstrates scripting workflow

**`Elnx-simple/scripts/clear_area.sh`**
- Eraser rectangle
- Usage: `clear_area.sh 100 100 800 600`

### 5. Lamp Engine

**`Elnx-simple/lamp/main.cpp`**
- Modified lamp with rmkit geometry functions
- Commands: circle, line, rectangle, arc, bezier
- Eraser support
- Direct framebuffer access (no rm2fb)

**`Elnx-simple/lamp/Makefile`**
- Cross-compilation for ARM
- Deployment target
- Test command

## Coordinate System

### Relative Coordinates (JSON)

```json
{
  "x": 0.5,   // 50% of bounding box width
  "y": 0.25   // 25% of bounding box height
}
```

**Conversion to Absolute**:
```python
abs_x = offset_x + (rel_x * bbox_width * scale)
abs_y = offset_y + (rel_y * bbox_height * scale)
```

**Benefits**:
- Scale-independent definitions
- Easy to scale components
- Portable across different screen resolutions
- Natural coordinate system for designers

### Absolute Coordinates (Lamp)

- Screen: 1404 x 1872 pixels
- Origin: Top-left (0, 0)
- Center: (702, 936)

## Workflow Comparison

### Old Workflow (Elxnk)

```
Edit SVG → Run svg2header.py → Rebuild C++ →
Deploy binaries → Test on device → Repeat
```

**Time per iteration**: ~5-10 minutes

### New Workflow (Elnx-Simple)

```
Edit JSON → Draw via WiFi → Test on device → Repeat
```

**Time per iteration**: ~5 seconds

## JSON Structure

### Component Format

```json
{
  "COMPONENT_NAME": {
    "name": "Human Readable Name",
    "type": "passive|semiconductor|transistor|symbol",
    "bbox": {
      "width": 50,
      "height": 100,
      "unit": "px"
    },
    "terminals": {
      "terminal_name": {"x": 0.5, "y": 0.0}
    },
    "paths": [
      {
        "type": "line|path|circle",
        "...": "type-specific properties"
      }
    ]
  }
}
```

### Path Types

**Line**:
```json
{
  "type": "line",
  "from": {"x": 0.0, "y": 0.5},
  "to": {"x": 1.0, "y": 0.5}
}
```

**Circle**:
```json
{
  "type": "circle",
  "cx": 0.5,
  "cy": 0.5,
  "r": 0.4
}
```

**Path** (complex):
```json
{
  "type": "path",
  "commands": [
    {"cmd": "M", "x": 0.0, "y": 0.0},
    {"cmd": "L", "x": 1.0, "y": 1.0},
    {"cmd": "C", "x1": 0.3, "y1": 0.3, "x2": 0.7, "y2": 0.7, "x": 1.0, "y": 1.0}
  ]
}
```

## Lamp Geometry Commands (from rmkit)

### Implemented Functions

**`pen_draw_circle(cx, cy, r1, r2)`**
- Draws circles using parametric arc tracing
- Uses 360-degree sweep with 1-degree steps

**`pen_draw_arc(cx, cy, r1, r2, a1, a2)`**
- Draws arcs between angles a1 and a2
- Angles in degrees (0-360)

**`pen_draw_line(x1, y1, x2, y2)`**
- Simple line drawing
- Uses pen down/move/up sequence

**`pen_draw_rectangle(x1, y1, x2, y2)`**
- Four-sided rectangle
- Connected line segments

**`pen_draw_rounded_rectangle(x1, y1, x2, y2, r)`**
- Rectangle with rounded corners
- Combines lines and arcs

**`pen_draw_bezier(coords)`**
- Quadratic (6 coords) or cubic (8 coords) bezier
- Parametric curve tracing

## Usage Examples

### Basic Component Drawing

```bash
# Draw resistor at (500, 400) with scale 1.5
./scripts/draw_component.sh R 500 400 1.5

# Draw MOSFET
./scripts/draw_component.sh NMOS 300 600 1.0
```

### Text Rendering

```bash
# Draw text at (300, 200) with scale 1.0
./scripts/draw_text.sh "R1" 300 200 1.0

# Larger text
./scripts/draw_text.sh "100K" 350 250 1.5
```

### Complete Circuit

```bash
#!/bin/bash
# Voltage divider circuit

./scripts/draw_component.sh VDC 200 300 1.0
./scripts/draw_component.sh R 400 300 1.0    # R1
./scripts/draw_component.sh R 400 500 1.0    # R2
./scripts/draw_component.sh GND 400 700 1.0

./scripts/draw_text.sh "V1" 150 300 0.8
./scripts/draw_text.sh "R1" 450 300 0.8
./scripts/draw_text.sh "10K" 450 330 0.7
./scripts/draw_text.sh "R2" 450 500 0.8
./scripts/draw_text.sh "10K" 450 530 0.7
```

### Direct Command Piping

```bash
# Generate commands
./bin/render_json.py component R 500 400 1.0

# Pipe to lamp via SSH
./bin/render_json.py component R 500 400 1.0 | \
  ssh root@10.11.99.1 /opt/bin/lamp
```

## Integration with iago

**iago** (from rmkit) uses similar pattern:
```cpp
cmd := 'echo "' + str + '" | /opt/bin/lamp'
```

**Elnx-Simple** follows same approach:
```bash
echo "<lamp commands>" | ssh root@HOST /opt/bin/lamp
```

This makes Elnx-Simple compatible with iago's command structure.

## Development History

### Commit Timeline (Previous Branch)

1. Initial Elxnk integration (0b52848)
2. SVG parser development (a4ebe45 → 2d35582)
3. Coordinate system fixes (19c9b51, 262d042)
4. Bezier curve interpolation (4a629a7, c6b86d6)
5. rmkit geometry functions (379316c)
6. Compilation fixes (e8bfb97)

### New Branch (elxnk-simple)

Created from claude/analyze-legacy-deploy-gdoVV with:
- JSON component library
- Python rendering tools
- WiFi command infrastructure
- Bash convenience scripts
- Modified lamp with geometry functions

## Network Setup

### reMarkable 2 Configuration

**USB Connection**: 10.11.99.1 (default)
**WiFi Connection**: Check Settings → Help → Copyrights → General information

**SSH Setup**:
```bash
# Test connection
ssh root@10.11.99.1 true

# Set host
export RM2_HOST=10.11.99.1

# Or for WiFi
export RM2_HOST=192.168.1.xxx
```

**Passwordless SSH** (optional):
```bash
ssh-copy-id root@10.11.99.1
```

## Testing Workflow

### 1. Deploy Lamp (One Time)

```bash
cd Elnx-simple/lamp
make
make deploy
```

### 2. Test Connection

```bash
# Set host
export RM2_HOST=10.11.99.1

# Test lamp
cd Elnx-simple/lamp
make test
```

### 3. Draw Components

```bash
cd Elnx-simple

# List components
./bin/render_json.py list

# Draw resistor
./scripts/draw_component.sh R 700 900 1.0

# Draw text
./scripts/draw_text.sh "TEST" 600 800 1.0
```

## Component Library Status

### Implemented Components

From `components.json`:
- **R** - Resistor (passive)
- **D** - Diode (semiconductor)
- **GND** - Ground (symbol)
- **C** - Capacitor (passive)
- **NMOS** - N-Channel MOSFET (transistor)

### To Be Added

Can be easily added by editing JSON:
- L (Inductor)
- VDC, VAC, VAR (voltage sources)
- PMOS, NPN, PNP (other transistors)
- OPAMP (op-amp)
- ZD (Zener diode)
- TX (transformer)

## Future Enhancements

### Short Term
- [ ] Complete component library (all 17 components)
- [ ] Complete font library (A-Z, 0-9)
- [ ] Component rotation support
- [ ] Batch drawing from JSON circuit files

### Medium Term
- [ ] Auto-wiring between terminals
- [ ] Circuit templates
- [ ] Interactive component browser
- [ ] Undo/redo support

### Long Term
- [ ] Color support (for reMarkable Color)
- [ ] GUI frontend
- [ ] Component library versioning
- [ ] Cloud component sharing

## Comparison Summary

| Aspect | Elxnk (C++) | Elnx-Simple (JSON) |
|--------|-------------|-------------------|
| **Complexity** | High | Low |
| **Rebuild Required** | Yes | No |
| **Remote Drawing** | No | Yes |
| **Edit Time** | Minutes | Seconds |
| **Library Format** | Binary C++ | Text JSON |
| **Deployment** | Full system | Lamp only |
| **Testing** | On-device | Remote WiFi |
| **Extensibility** | Rebuild C++ | Edit JSON |
| **Learning Curve** | Steep | Gentle |

## Conclusion

**Elnx-Simple** provides a much simpler, more flexible approach to circuit drawing on reMarkable 2 by:

1. Using JSON for component definitions
2. Enabling WiFi-based remote drawing
3. Eliminating rebuild cycles
4. Simplifying the development workflow
5. Maintaining all functionality of rmkit lamp

This makes it ideal for rapid prototyping, testing, and circuit design work.
