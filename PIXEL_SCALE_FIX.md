# Pixel-Scale Coordinate System - FIXED

## The Problem

**User Issue**: "The component library still has pen commands with coordinates not adequate. It should range for x:0-1404 & y: 0-1872."

The component library was storing coordinates in a tiny 0-10 normalized range which caused:
1. **Precision Loss**: Integer truncation losing sub-pixel detail
2. **Inadequate Scale**: Components too small, hard to scale up correctly
3. **Circle Radius Issues**: Small circles truncated to 0 pixels
4. **Poor Screen Utilization**: Coordinates not suitable for direct screen placement

## The Solution

Store coordinates in **PIXELS** at proper screen scale (10 pixels/mm matching reMarkable 2 DPI).

### Before vs After

#### Before (Normalized 0-10 Scale)
```c
// R component (resistor)
static const LampCommand R_commands[] = {
    {"pen down 1 2"},   // Tiny coordinates
    {"pen move 1 1"},   // Precision lost
    {"pen move 2 2"},   // Only 0-10 range
    ...
};

// NMOS circles
{"pen circle 7 5 3 3"},      // Large circle (barely visible)
{"pen circle 8 3 0 0"},      // Small circle INVISIBLE (radius 0!)
```

**Problems**:
- R component: 1x8 pixels (way too small)
- Circle radius 0 = invisible
- Scaling up from 0-10 → magnifies rounding errors

#### After (Pixel Scale at 10px/mm)
```c
// R component (resistor)
static const LampCommand R_commands[] = {
    {"pen down 11 16"},   // Proper pixel coordinates
    {"pen move 11 13"},   // Full precision preserved
    {"pen move 21 20"},   // Spans 22x78 pixels
    ...
};

// NMOS circles
{"pen circle 68 52 28 28"},  // Large circle (28 pixel radius - clear)
{"pen circle 77 34 3 3"},    // Small circle (3 pixels - visible dot)
```

**Fixed**:
- R component: 22x78 pixels (actual 2.18mm x 7.77mm size)
- Circles: radius 3-28 pixels (all visible)
- Direct screen placement anywhere in 0-1404 x 0-1872 range

## Coordinate System Architecture

### 1. SVG Source (Millimeters)
```xml
<!-- R.svg: 2.18mm wide x 7.77mm tall -->
<svg width="2.18mm" height="7.77mm" viewBox="0 0 2.18 7.77">
  <g transform="translate(-164.00781,-105.3338)">
    <path d="M 165.1,106.934 V 106.68" />
    <!-- Coordinates in mm with transform -->
  </g>
</svg>
```

### 2. Build Time: svg2header.py
```python
PIXELS_PER_MM = 10.0  # reMarkable 2 ~8.9 px/mm, using 10 for clean math

# Apply SVG transform, convert mm to pixels
cx_mm = cx + transform_x       # 165.1 - 164.0 = 1.1 mm
cx_pixels = round(cx_mm * 10)  # 1.1 * 10 = 11 pixels
```

**Output**: `src/elxnk/component_library.h` with pixel coordinates

### 3. Runtime: render_component.cpp
```cpp
// Library has: pen down 11 16 (pixels at 10px/mm scale)
// User calls: render_component("R", 500, 500, 1.0)

// Transform: (library_x * scale) + offset
cx = (11 * 1.0) + 500 = 511
cy = (16 * 1.0) + 500 = 516

// Send to lamp: pen down 511 516 (absolute screen pixels)
```

### 4. Lamp Draws on Screen
```
Screen: 1404 x 1872 pixels (reMarkable 2)
Draws at: (511, 516) absolute pixel coordinates
Component size: 22x78 pixels (1:1 scale)
```

## Examples

### Example 1: R Resistor at 1:1 Scale
```bash
render_component R 500 500 1.0
```
- **Library**: "pen down 11 16" (11 pixels from component origin)
- **Transform**: (11 * 1.0) + 500 = 511
- **Lamp gets**: "pen down 511 516"
- **Result**: 22x78 pixel resistor at screen position (500, 500)

### Example 2: NMOS Transistor at 2x Scale
```bash
render_component NMOS 700 400 2.0
```
- **Library**: "pen down 54 33", "pen circle 68 52 28 28"
- **Transform**:
  - Point: (54 * 2.0) + 700 = 808, (33 * 2.0) + 400 = 466
  - Circle: center (68 * 2.0) + 700 = 836, (52 * 2.0) + 400 = 504
  - Radius: 28 * 2.0 = 56 pixels
- **Lamp gets**: "pen down 808 466", "pen circle 836 504 56 56"
- **Result**: 194x206 pixel transistor (2x original size)

### Example 3: Components Across Full Screen
```bash
# Top left
render_component R 50 50 1.0     → draws at pixels 61-127

# Center
render_component NMOS 702 936 1.0 → draws at screen center

# Bottom right
render_component VDC 1300 1800 1.0 → draws near bottom-right corner
```

All components can be placed anywhere in the 1404x1872 screen space.

## Component Sizes

All sizes at 1:1 scale (scale=1.0):

| Component | SVG Size (mm) | Pixel Size | Coord Range |
|-----------|---------------|------------|-------------|
| R (resistor) | 2.18 x 7.77 | 22 x 78 | 1-21, 13-77 |
| NMOS | 9.73 x 10.31 | 97 x 103 | 0-97, 26-103 |
| PMOS | 9.58 x 10.19 | 96 x 102 | 0-96, 28-102 |
| D (diode) | 5.08 x 7.87 | 51 x 79 | 32-83, 4-83 |
| VDC | 5.33 x 7.62 | 53 x 76 | 0-53, 0-76 |

Circles:
- Large circles (transistor bodies): 25-28 pixel radius (2.5-2.8mm)
- Small circles (terminal dots): 3 pixel radius (0.254mm)

## Scaling Behavior

The `scale` parameter in `render_component(name, x, y, scale)`:

| Scale | R Size | Use Case |
|-------|--------|----------|
| 0.5 | 11x39 pixels | Small schematic, many components |
| 1.0 | 22x78 pixels | Standard size (default) |
| 2.0 | 44x156 pixels | Large, detailed view |
| 5.0 | 110x390 pixels | Zoom view, demonstrations |

## reMarkable 2 DPI Calculation

```
Screen: 1404 x 1872 pixels
Physical: 157.2 x 209.6 mm
DPI: √(1404² + 1872²) / √(157.2² + 209.6²) * 25.4 = 226 DPI
Pixels per mm: 226 / 25.4 = 8.898 ≈ 9 px/mm (actual)

We use 10 px/mm for:
- Cleaner integer math
- Slightly larger components (more readable)
- Easy mental conversion (2.5mm = 25 pixels)
```

## Files Modified

### 1. tools/svg2header.py
**Key Change**: Added `PIXELS_PER_MM = 10.0` and multiply all coordinates:

```python
# Before
final_x = int(round(current_x + tx))

# After
final_x = int(round((current_x + tx) * PIXELS_PER_MM))
```

Applied to all functions: `parse_path_data()`, `parse_circle()`, `parse_rect()`, `parse_line()`

### 2. src/elxnk/component_library.h
**Generated Output**: All coordinates multiplied by 10

```c
// Before: 707 commands, coordinates 0-10
// After:  707 commands, coordinates 0-103 (pixel scale)
```

Command count unchanged (still efficient with circles).

### 3. src/elxnk/render_component.cpp
**Comment Updates**: Clarified new coordinate system

```cpp
// Old comment: "Component library: Normalized coordinates (e.g., 0-10)"
// New comment: "Component library: Pixel coordinates at 10 px/mm scale"
```

**Transform logic unchanged**: Still does `(coord * scale) + offset`
- Now more meaningful: `(11 pixels * 2.0) + 500 = 522` instead of `(1 unit * 10) + 500 = 510`

## Testing

### On Device
```bash
# SSH to reMarkable
ssh root@10.11.99.1

# Test single component (1:1 scale)
render_component R 700 900 1.0

# Test with scaling
render_component NMOS 500 500 2.0

# Test all components
test_components.sh
```

### Expected Results
- ✅ Components render at correct sizes
- ✅ Circles visible and correctly sized
- ✅ No precision artifacts or rounding errors
- ✅ Components can be placed anywhere on screen (0-1404, 0-1872)
- ✅ Scaling works smoothly (0.5x to 5.0x)

## Build Instructions

```bash
cd src

# Regenerate library with pixel coordinates
make clean library

# Build binaries (requires ARM cross-compiler)
make all

# Deploy to device
make deploy HOST=10.11.99.1
```

## Why 10 Pixels/mm?

1. **Close to Actual DPI**: reMarkable 2 is ~8.9 px/mm, so 10 is only 12% larger
2. **Clean Mental Math**: 1mm = 10 pixels, 2.5mm = 25 pixels
3. **Integer-Friendly**: Reduces floating point rounding in calculations
4. **Readable Size**: Slightly larger components are easier to see on e-ink
5. **Standard Scale**: Common in graphics (96 DPI ≈ 3.78 px/mm, we're 2.6x for tablet)

## Verification

Check a few key coordinates to verify the fix:

```bash
# R component should span ~22x78 pixels
grep -A 5 "// Component: R$" src/elxnk/component_library.h

# NMOS should have circles with radius 28 and 3
grep "pen circle" src/elxnk/component_library.h | grep NMOS -A 3

# Coordinates should be in 0-100 range (not 0-10)
grep "pen down" src/elxnk/component_library.h | head -20
```

Expected output:
```c
{"pen down 11 16"},  // ✓ In pixel range (not 1 1)
{"pen circle 68 52 28 28"},  // ✓ Proper radius (not 0 or 3)
```

## Summary

| Aspect | Before | After |
|--------|--------|-------|
| **Coordinate Scale** | 0-10 normalized | 10-100 pixels |
| **R Component Size** | 1x8 (inadequate) | 22x78 pixels |
| **Circle Radius** | 0-3 (many invisible) | 3-28 pixels (all visible) |
| **Precision** | Low (integer truncation) | High (sub-mm preserved) |
| **Screen Range** | Limited | Full 0-1404 x 0-1872 |
| **Scaling Quality** | Poor (magnifies errors) | Excellent |

**Result**: Components now have adequate pixel coordinates suitable for direct screen placement and rendering at any position in the 0-1404 x 0-1872 screen space.
