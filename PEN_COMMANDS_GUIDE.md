# Pen Commands Build System Guide

## CRITICAL: Lamp Uses Absolute Screen Coordinates

**reMarkable 2 Display**: 1404 x 1872 pixels

**Lamp command format**:
- `pen down X Y` - X and Y are ABSOLUTE pixel coordinates (0-1404, 0-1872)
- `pen move X Y` - X and Y are ABSOLUTE pixel coordinates
- `pen up` - No coordinates

**Coordinate Flow**:
1. **Component Library**: Normalized coordinates (e.g., `pen down 1 1`)
2. **render_component**: Transforms to absolute screen pixels: `(1 * scale) + offset_x`
3. **Lamp**: Receives absolute coordinates (e.g., `pen down 502 502`)

**Example**:
```bash
render_component R 500 500 2.0
# Component lib: pen down 1 1
# Transform: (1 * 2.0) + 500 = 502, (1 * 2.0) + 500 = 502
# Lamp gets: pen down 502 502  (absolute screen pixel)
```

## Where Pen Commands Are Built

The pen commands for all components and fonts are built during compilation and stored in:

```
src/elxnk/component_library.h  (Auto-generated - DO NOT EDIT MANUALLY)
```

This header file contains:
- **17 Components**: R, C, L, D, NMOS, PMOS, NPN, PNP, OPAMP, etc.
- **36 Font Glyphs**: A-Z, 0-9
- **1072 Total Commands**: All pen commands embedded as C arrays

### File Locations

| File | Purpose | Location |
|------|---------|----------|
| **Component Library (Generated)** | Embedded pen commands | `src/elxnk/component_library.h` |
| **SVG Parser (Build Tool)** | Converts SVGs to C header | `tools/svg2header.py` |
| **Standalone Converter** | Test individual SVG files | `tools/svg_to_lamp_smartv2.py` |
| **SVG Assets** | Source component SVGs | `assets/components/*.svg` |
| **Font Assets** | Source font SVGs | `assets/font/*.svg` |

## How to Regenerate Pen Commands

### Method 1: Full Build (Recommended)

```bash
cd src
make clean library
```

This:
1. Deletes old `component_library.h`
2. Runs `svg2header.py` on all 54 SVG files
3. Generates new `component_library.h` with all pen commands

### Method 2: Test Single Component

Use the standalone converter to see pen commands for one SVG:

```bash
python3 tools/svg_to_lamp_smartv2.py assets/components/R.svg

# With offset and scale:
python3 tools/svg_to_lamp_smartv2.py assets/components/R.svg 500 500 2.0

# Outputs:
pen up
pen move 1 1
pen down
pen move 1 1
pen up
...
```

## Parsing Logic Explained

### Coordinate System

SVG files use absolute coordinates with group transforms. The parser:

1. **Reads SVG coordinates** in untransformed space (e.g., `165.1, 106.934`)
2. **Extracts group transform** from `<g transform="translate(-164,-105)">`
3. **Normalizes coordinates** by applying transform: `(165+(-164), 106+(-105)) = (1, 1)`
4. **Outputs pen commands** with small normalized coordinates

### Example SVG Parse

```xml
<g transform="translate(-164.00781,-105.3338)">
  <path d="M 165.1,106.934 V 106.68"/>
  <path d="m 165.1,106.934 1.016,0.381 -1.016,0.381"/>
</g>
```

**Parsing Flow:**
```
Path 1 (M = absolute):
  - Read: (165.1, 106.934)
  - Apply transform: (165.1 + (-164), 106.934 + (-105)) = (1.1, 1.6)
  - Output: pen move 1 1

Path 2 (m = relative from origin):
  - Start at (0, 0)
  - Move by (165.1, 106.934) → position (165.1, 106.934)
  - Apply transform: (1.1, 1.6)
  - Output: pen move 1 1
  - Next relative move (+1.016, +0.381) → position (166.116, 107.315)
  - Apply transform: (2.1, 2.0)
  - Output: pen move 2 2
```

### Pen State Management

The parser tracks pen state to minimize redundant commands:

```python
pen_is_down = False

# At Move command (M/m)
if pen_is_down:
    commands.append("pen up")  # Lift before moving
    pen_is_down = False

commands.append("pen move X Y")  # Move without drawing

# At Line command (L/l)
if not pen_is_down:
    commands.append("pen down")  # Lower pen before drawing
    pen_is_down = True

commands.append("pen move X Y")  # Draw line
```

## Modifying the Parser

### To Change Parsing Logic

Edit: `tools/svg2header.py`

Key function: `parse_path_data(path_data, tx, ty, is_first=False)`

**Common modifications:**

1. **Add new SVG command support**:
```python
elif cmd in 'Qq':  # Quadratic Bezier
    if not pen_is_down:
        commands.append("pen down")
        pen_is_down = True
    # Process coordinates...
```

2. **Change coordinate transformation**:
```python
# Current: Apply group translate
final_x = int(current_x + tx)
final_y = int(current_y + ty)

# Example: Add scaling
final_x = int((current_x + tx) * scale)
final_y = int((current_y + ty) * scale)
```

3. **Add pen pressure/tilt**:
```python
commands.append(f"pen move {final_x} {final_y} pressure 1700 tilt 50,-150")
```

### To Use Early Converter Versions

If you have an earlier version of the converter (e.g., `svg_to_lamp_v1.py`):

1. **Place it in `tools/`**:
```bash
cp /path/to/svg_to_lamp_v1.py tools/
```

2. **Modify Makefile** to use it:
```makefile
# In src/Makefile, change this line:
$(PYTHON) $(TOOLS_DIR)/svg2header.py \    # Current

# To:
$(PYTHON) $(TOOLS_DIR)/svg_to_lamp_v1.py \  # Your version
```

3. **Ensure same output format**:
Your converter must generate valid C header with this structure:
```c
static const LampCommand R_commands[] = {
    {"pen up"},
    {"pen move 1 1"},
    {"pen down"},
    ...
};
```

## Testing Pen Commands

### Test on Device

```bash
# Deploy to reMarkable
cd src
make deploy HOST=10.11.99.1

# SSH to device
ssh root@10.11.99.1

# Test single component
render_component R 500 500

# Test all components
test_components.sh

# Test fonts
test_fonts.sh
```

### Verify Commands Locally

```bash
# Check generated commands for component R
grep -A 40 "// Component: R" src/elxnk/component_library.h

# Count total commands
grep -c '{"pen' src/elxnk/component_library.h

# List all components
grep "^// Component:" src/elxnk/component_library.h
```

## Bash Script Line Endings

All bash scripts use **LF line endings** (Unix style):

```bash
# Verify:
file src/*.sh
# Output: ASCII text executable (not "CRLF")

# Convert if needed:
dos2unix src/*.sh
```

**Scripts with LF endings:**
- `src/test_components.sh`
- `src/test_fonts.sh`
- `src/ui_state.sh`

## Common Issues

### Issue 1: Lines from 0,0 to Component

**Symptom**: Components draw lines from origin before rendering correctly

**Cause**: Missing `pen up` before first `pen move`

**Fix**: Parser now automatically adds `pen up` at start of each path (line 135 in svg2header.py)

### Issue 2: Large Coordinates (165, 106 instead of 1, 1)

**Symptom**: Coordinates not normalized to component origin

**Cause**: Transform not applied correctly

**Fix**: Parser tracks in untransformed space, applies transform on output (lines 120-132)

### Issue 3: Duplicate "pen up" Commands

**Symptom**: Multiple consecutive `pen up` in output

**Cause**: Both path end and next path start adding `pen up`

**Impact**: Harmless but wasteful. Current parser minimizes this.

## Build System Flow

```
┌─────────────────────────────────────────────────────────┐
│ 1. SVG ASSETS                                           │
│    - assets/components/*.svg (17 components)            │
│    - assets/font/*.svg (36 fonts)                       │
└────────────────┬────────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────────┐
│ 2. BUILD PROCESS (make library)                         │
│    tools/svg2header.py parses all SVGs                  │
│    - Extracts path data                                 │
│    - Applies transforms                                 │
│    - Generates pen commands                             │
└────────────────┬────────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────────┐
│ 3. GENERATED HEADER                                     │
│    src/elxnk/component_library.h (27KB, 1072 commands)  │
│    - C arrays: R_commands[], C_commands[], etc          │
│    - Lookup tables: COMPONENTS[], FONT_GLYPHS[]         │
│    - Helper functions: find_component(), find_glyph()   │
└────────────────┬────────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────────┐
│ 4. COMPILATION (make render)                            │
│    src/elxnk/render_component.cpp includes header       │
│    Compiled → build/bin/render_component binary         │
└────────────────┬────────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────────┐
│ 5. DEPLOYMENT (make install)                            │
│    Binary → /opt/bin/render_component on reMarkable 2   │
│    Test scripts → /opt/bin/test_*.sh                    │
└────────────────┬────────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────────┐
│ 6. RUNTIME                                              │
│    render_component R 500 500                           │
│    → Reads embedded commands from binary               │
│    → Sends to /tmp/elxnk_lamp.pipe                      │
│    → lamp draws on screen                               │
└─────────────────────────────────────────────────────────┘
```

## Quick Reference

| Task | Command |
|------|---------|
| Regenerate all pen commands | `cd src && make library` |
| Test single SVG | `python3 tools/svg_to_lamp_smartv2.py assets/components/R.svg` |
| View generated commands | `grep -A 40 "// Component: R" src/elxnk/component_library.h` |
| Check line endings | `file src/*.sh` |
| Deploy to device | `cd src && make deploy HOST=10.11.99.1` |
| Test on device | `ssh root@10.11.99.1 render_component R 500 500` |
| List all components | `render_component list` (on device) |
| Full rebuild | `cd src && make clean all install HOST=10.11.99.1` |
