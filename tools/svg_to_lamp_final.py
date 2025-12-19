#!/usr/bin/env python3
"""
SVG to LAMP Complete Converter - Final Version
===============================================

Addresses all previous parser issues:
- ✓ Handles negative coordinates correctly
- ✓ Parses ALL path commands (M, L, H, V, C, S, Q, T, A, Z)
- ✓ Supports circles, rectangles, lines
- ✓ Uses svgpathtools for reliable curve parsing
- ✓ Minimal commands via intelligent simplification
- ✓ Pixel-based coordinates (10 px/mm for reMarkable 2)
- ✓ Auto-centering and scaling
- ✓ Header file generation for embedded C compilation

Usage:
  Single file test: python3 svg_to_lamp_final.py file.svg [scale] [offset_x] [offset_y]
  Generate header: python3 svg_to_lamp_final.py --header components/ fonts/ output.h
"""

import sys
import re
import math
from pathlib import Path
import xml.etree.ElementTree as ET
from typing import List, Tuple, Dict

try:
    from svgpathtools import parse_path, Line, Arc, CubicBezier, QuadraticBezier, Path
    HAS_SVGPATHTOOLS = True
except ImportError:
    HAS_SVGPATHTOOLS = False
    print("Warning: svgpathtools not available, curve parsing will be limited", file=sys.stderr)

# reMarkable 2 constants
SCREEN_WIDTH = 1404
SCREEN_HEIGHT = 1872
PIXELS_PER_MM = 10.0  # Clean scaling factor


class SVGToLamp:
    """Comprehensive SVG parser with all element support"""
    
    def __init__(self, scale: float = None, offset_x: int = None, offset_y: int = None):
        self.scale = scale
        self.offset_x = offset_x
        self.offset_y = offset_y
        self.bounds = None
        
    def parse_file(self, svg_path: str) -> List[str]:
        """Parse SVG file and return lamp commands"""
        tree = ET.parse(svg_path)
        root = tree.getroot()
        
        # Calculate bounds first for auto-scaling/centering
        self.bounds = self._calculate_bounds(root)
        
        # Auto-configure if not set
        if self.scale is None or self.offset_x is None or self.offset_y is None:
            self._auto_configure()
        
        # Parse all elements
        commands = []
        commands.extend(self._parse_element(root))
        
        return commands
    
    def _auto_configure(self):
        """Auto-calculate scale and offset for centering"""
        if not self.bounds:
            self.scale = PIXELS_PER_MM
            self.offset_x = SCREEN_WIDTH // 2
            self.offset_y = SCREEN_HEIGHT // 2
            return
        
        minx, miny, maxx, maxy = self.bounds
        width = maxx - minx
        height = maxy - miny
        
        if width <= 0 or height <= 0:
            self.scale = PIXELS_PER_MM
            self.offset_x = SCREEN_WIDTH // 2
            self.offset_y = SCREEN_HEIGHT // 2
            return
        
        # Auto-scale to fit with margin
        MARGIN = 100
        if self.scale is None:
            scale_x = (SCREEN_WIDTH - 2 * MARGIN) / width
            scale_y = (SCREEN_HEIGHT - 2 * MARGIN) / height
            self.scale = min(scale_x, scale_y)
        
        # Auto-center
        if self.offset_x is None:
            self.offset_x = int((SCREEN_WIDTH - width * self.scale) / 2 - minx * self.scale)
        if self.offset_y is None:
            self.offset_y = int((SCREEN_HEIGHT - height * self.scale) / 2 - miny * self.scale)
    
    def _transform_point(self, x: float, y: float) -> Tuple[int, int]:
        """Transform SVG coordinate to screen pixel"""
        px = int(round(x * self.scale + self.offset_x))
        py = int(round(y * self.scale + self.offset_y))
        # Clamp to screen bounds
        px = max(0, min(px, SCREEN_WIDTH - 1))
        py = max(0, min(py, SCREEN_HEIGHT - 1))
        return px, py
    
    def _calculate_bounds(self, root) -> Tuple[float, float, float, float]:
        """Calculate bounding box of all drawable elements"""
        minx = miny = float('inf')
        maxx = maxy = float('-inf')
        
        for elem in root.iter():
            tag = elem.tag.split('}')[-1]
            
            # Skip hidden or pin elements
            elem_id = elem.get('id', '')
            if 'pin' in elem_id.lower() or elem.get('opacity') == '0':
                continue
            
            try:
                if tag == 'path':
                    d = elem.get('d', '')
                    if d and HAS_SVGPATHTOOLS:
                        path = parse_path(d)
                        bbox = path.bbox()
                        if bbox:
                            minx = min(minx, bbox[0])
                            maxx = max(maxx, bbox[1])
                            miny = min(miny, bbox[2])
                            maxy = max(maxy, bbox[3])
                
                elif tag == 'circle':
                    cx = float(elem.get('cx', 0))
                    cy = float(elem.get('cy', 0))
                    r = float(elem.get('r', 0))
                    minx = min(minx, cx - r)
                    maxx = max(maxx, cx + r)
                    miny = min(miny, cy - r)
                    maxy = max(maxy, cy + r)
                
                elif tag == 'rect':
                    x = float(elem.get('x', 0))
                    y = float(elem.get('y', 0))
                    w = float(elem.get('width', 0))
                    h = float(elem.get('height', 0))
                    minx = min(minx, x)
                    maxx = max(maxx, x + w)
                    miny = min(miny, y)
                    maxy = max(maxy, y + h)
                
                elif tag == 'line':
                    x1 = float(elem.get('x1', 0))
                    y1 = float(elem.get('y1', 0))
                    x2 = float(elem.get('x2', 0))
                    y2 = float(elem.get('y2', 0))
                    minx = min(minx, x1, x2)
                    maxx = max(maxx, x1, x2)
                    miny = min(miny, y1, y2)
                    maxy = max(maxy, y1, y2)
            
            except Exception as e:
                continue
        
        if minx == float('inf'):
            return (0, 0, 100, 100)
        
        return (minx, miny, maxx, maxy)
    
    def _parse_element(self, elem, parent_transform=None) -> List[str]:
        """Recursively parse SVG elements"""
        commands = []
        
        # Get element transform
        transform = self._parse_transform(elem.get('transform', ''))
        if parent_transform:
            transform = self._combine_transforms(parent_transform, transform)
        
        tag = elem.tag.split('}')[-1]
        elem_id = elem.get('id', '')
        
        # Skip hidden or pin elements
        if 'pin' in elem_id.lower() or elem.get('opacity') == '0':
            return commands
        
        # Parse based on element type
        if tag == 'path':
            commands.extend(self._parse_path(elem, transform))
        elif tag == 'circle':
            commands.extend(self._parse_circle(elem, transform))
        elif tag == 'rect':
            commands.extend(self._parse_rect(elem, transform))
        elif tag == 'line':
            commands.extend(self._parse_line(elem, transform))
        elif tag == 'polyline' or tag == 'polygon':
            commands.extend(self._parse_poly(elem, transform, tag == 'polygon'))
        elif tag in ('g', 'svg'):
            # Recursively process children
            for child in elem:
                commands.extend(self._parse_element(child, transform))
        
        return commands
    
    def _parse_transform(self, transform_str: str) -> Dict:
        """Parse SVG transform attribute"""
        result = {'translate': (0, 0), 'scale': (1, 1), 'rotate': 0}
        
        if not transform_str:
            return result
        
        # Parse translate
        match = re.search(r'translate\s*\(\s*([^,\)]+)\s*(?:,\s*([^)]+))?\s*\)', transform_str)
        if match:
            tx = float(match.group(1))
            ty = float(match.group(2)) if match.group(2) else 0
            result['translate'] = (tx, ty)
        
        # Parse scale
        match = re.search(r'scale\s*\(\s*([^,\)]+)\s*(?:,\s*([^)]+))?\s*\)', transform_str)
        if match:
            sx = float(match.group(1))
            sy = float(match.group(2)) if match.group(2) else sx
            result['scale'] = (sx, sy)
        
        # Parse rotate
        match = re.search(r'rotate\s*\(\s*([^)]+)\s*\)', transform_str)
        if match:
            result['rotate'] = float(match.group(1))
        
        return result
    
    def _combine_transforms(self, parent: Dict, child: Dict) -> Dict:
        """Combine parent and child transforms"""
        return {
            'translate': (
                parent['translate'][0] + child['translate'][0],
                parent['translate'][1] + child['translate'][1]
            ),
            'scale': (
                parent['scale'][0] * child['scale'][0],
                parent['scale'][1] * child['scale'][1]
            ),
            'rotate': parent['rotate'] + child['rotate']
        }
    
    def _apply_transform(self, x: float, y: float, transform: Dict) -> Tuple[float, float]:
        """Apply SVG transform to point"""
        if not transform:
            return x, y
        
        # Apply translate
        tx, ty = transform['translate']
        x += tx
        y += ty
        
        # Apply scale
        sx, sy = transform['scale']
        x *= sx
        y *= sy
        
        # Rotate not commonly needed for schematics, skip for simplicity
        
        return x, y
    
    def _parse_path(self, elem, transform) -> List[str]:
        """Parse SVG path element with full command support"""
        d = elem.get('d', '')
        if not d:
            return []
        
        if not HAS_SVGPATHTOOLS:
            return self._parse_path_fallback(d, transform)
        
        try:
            path = parse_path(d)
            return self._convert_path_to_commands(path, transform)
        except Exception as e:
            print(f"Warning: Path parsing failed: {e}", file=sys.stderr)
            return []
    
    def _convert_path_to_commands(self, path: Path, transform) -> List[str]:
        """Convert svgpathtools Path to lamp commands"""
        commands = []
        
        if not path:
            return commands
        
        # Sample the path into points
        points = self._sample_path(path)
        
        if not points:
            return commands
        
        # Apply transform and convert to screen coordinates
        transformed_points = []
        for x, y in points:
            tx, ty = self._apply_transform(x, y, transform)
            px, py = self._transform_point(tx, ty)
            transformed_points.append((px, py))
        
        # Simplify to remove redundant collinear points
        simplified = self._simplify_points(transformed_points)
        
        if not simplified:
            return commands
        
        # Generate pen commands
        commands.append(f"pen down {simplified[0][0]} {simplified[0][1]}")
        for px, py in simplified[1:]:
            commands.append(f"pen move {px} {py}")
        commands.append("pen up")
        
        return commands
    
    def _sample_path(self, path: Path, tolerance: float = 1.0) -> List[Tuple[float, float]]:
        """Sample path into points with adaptive density"""
        points = []
        
        for segment in path:
            if isinstance(segment, Line):
                # Straight line: only need endpoints
                points.append((segment.start.real, segment.start.imag))
                points.append((segment.end.real, segment.end.imag))
            else:
                # Curve: sample based on length
                seg_len = segment.length()
                
                # Adaptive sampling
                if seg_len < 5:
                    n = 3
                elif seg_len < 20:
                    n = 5
                elif seg_len < 50:
                    n = 8
                else:
                    n = max(8, min(15, int(seg_len * 0.15)))
                
                for i in range(n + 1):
                    t = i / n
                    pt = segment.point(t)
                    points.append((pt.real, pt.imag))
        
        return points
    
    def _simplify_points(self, points: List[Tuple[int, int]], tolerance: float = 2.0) -> List[Tuple[int, int]]:
        """Remove redundant collinear points using Douglas-Peucker"""
        if len(points) <= 2:
            return points
        
        def perpendicular_distance(point, line_start, line_end):
            """Calculate perpendicular distance from point to line"""
            x0, y0 = point
            x1, y1 = line_start
            x2, y2 = line_end
            
            dx = x2 - x1
            dy = y2 - y1
            
            if dx == 0 and dy == 0:
                return math.sqrt((x0 - x1)**2 + (y0 - y1)**2)
            
            return abs(dy * x0 - dx * y0 + x2 * y1 - y2 * x1) / math.sqrt(dx**2 + dy**2)
        
        def douglas_peucker(pts, epsilon):
            """Douglas-Peucker line simplification"""
            if len(pts) <= 2:
                return pts
            
            # Find point with maximum distance
            dmax = 0
            index = 0
            for i in range(1, len(pts) - 1):
                d = perpendicular_distance(pts[i], pts[0], pts[-1])
                if d > dmax:
                    index = i
                    dmax = d
            
            # If max distance is greater than epsilon, recursively simplify
            if dmax > epsilon:
                rec1 = douglas_peucker(pts[:index + 1], epsilon)
                rec2 = douglas_peucker(pts[index:], epsilon)
                return rec1[:-1] + rec2
            else:
                return [pts[0], pts[-1]]
        
        return douglas_peucker(points, tolerance)
    
    def _parse_circle(self, elem, transform) -> List[str]:
        """Parse circle element"""
        try:
            cx = float(elem.get('cx', 0))
            cy = float(elem.get('cy', 0))
            r = float(elem.get('r', 0))
            
            # Apply transform
            cx, cy = self._apply_transform(cx, cy, transform)
            
            # Transform to screen coordinates
            px, py = self._transform_point(cx, cy)
            pr = int(round(r * self.scale))
            pr = max(1, pr)  # Minimum 1 pixel
            
            return [f"pen circle {px} {py} {pr} {pr}"]
        
        except Exception as e:
            print(f"Warning: Circle parsing failed: {e}", file=sys.stderr)
            return []
    
    def _parse_rect(self, elem, transform) -> List[str]:
        """Parse rectangle as path"""
        try:
            x = float(elem.get('x', 0))
            y = float(elem.get('y', 0))
            w = float(elem.get('width', 0))
            h = float(elem.get('height', 0))
            
            # Four corners
            corners = [
                (x, y),
                (x + w, y),
                (x + w, y + h),
                (x, y + h)
            ]
            
            # Apply transform and convert
            screen_corners = []
            for cx, cy in corners:
                tx, ty = self._apply_transform(cx, cy, transform)
                px, py = self._transform_point(tx, ty)
                screen_corners.append((px, py))
            
            # Generate commands
            commands = [f"pen down {screen_corners[0][0]} {screen_corners[0][1]}"]
            for px, py in screen_corners[1:]:
                commands.append(f"pen move {px} {py}")
            commands.append(f"pen move {screen_corners[0][0]} {screen_corners[0][1]}")
            commands.append("pen up")
            
            return commands
        
        except Exception as e:
            print(f"Warning: Rect parsing failed: {e}", file=sys.stderr)
            return []
    
    def _parse_line(self, elem, transform) -> List[str]:
        """Parse line element"""
        try:
            x1 = float(elem.get('x1', 0))
            y1 = float(elem.get('y1', 0))
            x2 = float(elem.get('x2', 0))
            y2 = float(elem.get('y2', 0))
            
            # Apply transform
            tx1, ty1 = self._apply_transform(x1, y1, transform)
            tx2, ty2 = self._apply_transform(x2, y2, transform)
            
            # Convert to screen
            px1, py1 = self._transform_point(tx1, ty1)
            px2, py2 = self._transform_point(tx2, ty2)
            
            return [
                f"pen down {px1} {py1}",
                f"pen move {px2} {py2}",
                "pen up"
            ]
        
        except Exception as e:
            print(f"Warning: Line parsing failed: {e}", file=sys.stderr)
            return []
    
    def _parse_poly(self, elem, transform, closed: bool) -> List[str]:
        """Parse polyline/polygon"""
        try:
            points_str = elem.get('points', '')
            coords = [float(x) for x in re.findall(r'-?\d*\.?\d+', points_str)]
            
            if len(coords) < 4:
                return []
            
            # Convert pairs to points
            points = [(coords[i], coords[i+1]) for i in range(0, len(coords), 2)]
            
            # Apply transform and convert
            screen_points = []
            for x, y in points:
                tx, ty = self._apply_transform(x, y, transform)
                px, py = self._transform_point(tx, ty)
                screen_points.append((px, py))
            
            # Generate commands
            commands = [f"pen down {screen_points[0][0]} {screen_points[0][1]}"]
            for px, py in screen_points[1:]:
                commands.append(f"pen move {px} {py}")
            
            if closed:
                commands.append(f"pen move {screen_points[0][0]} {screen_points[0][1]}")
            
            commands.append("pen up")
            
            return commands
        
        except Exception as e:
            print(f"Warning: Polyline parsing failed: {e}", file=sys.stderr)
            return []
    
    def _parse_path_fallback(self, d: str, transform) -> List[str]:
        """Fallback path parser without svgpathtools"""
        # Basic path parsing for M, L commands only
        commands = []
        tokens = re.findall(r'[MmLlHhVvZz]|[-+]?\d*\.?\d+', d)
        
        i = 0
        current_x = current_y = 0.0
        start_x = start_y = 0.0
        pen_down = False
        
        while i < len(tokens):
            if i >= len(tokens):
                break
            
            cmd = tokens[i]
            if not cmd[0].isalpha():
                i += 1
                continue
            
            coords = []
            i += 1
            while i < len(tokens) and not tokens[i][0].isalpha():
                coords.append(float(tokens[i]))
                i += 1
            
            if cmd in 'Mm':
                if len(coords) >= 2:
                    if pen_down:
                        commands.append("pen up")
                        pen_down = False
                    
                    if cmd == 'M':
                        current_x, current_y = coords[0], coords[1]
                    else:
                        current_x += coords[0]
                        current_y += coords[1]
                    
                    start_x, start_y = current_x, current_y
                    tx, ty = self._apply_transform(current_x, current_y, transform)
                    px, py = self._transform_point(tx, ty)
                    commands.append(f"pen down {px} {py}")
                    pen_down = True
            
            elif cmd in 'Ll':
                if not pen_down:
                    tx, ty = self._apply_transform(current_x, current_y, transform)
                    px, py = self._transform_point(tx, ty)
                    commands.append(f"pen down {px} {py}")
                    pen_down = True
                
                j = 0
                while j + 1 < len(coords):
                    if cmd == 'L':
                        current_x, current_y = coords[j], coords[j+1]
                    else:
                        current_x += coords[j]
                        current_y += coords[j+1]
                    
                    tx, ty = self._apply_transform(current_x, current_y, transform)
                    px, py = self._transform_point(tx, ty)
                    commands.append(f"pen move {px} {py}")
                    j += 2
            
            elif cmd in 'Zz':
                if pen_down:
                    current_x, current_y = start_x, start_y
                    tx, ty = self._apply_transform(current_x, current_y, transform)
                    px, py = self._transform_point(tx, ty)
                    commands.append(f"pen move {px} {py}")
        
        if pen_down:
            commands.append("pen up")
        
        return commands


def generate_header_file(components_dir: str, fonts_dir: str, output_file: str):
    """Generate C header file with all components and fonts"""
    
    # Use fixed scale for consistent component sizes
    parser = SVGToLamp(scale=PIXELS_PER_MM, offset_x=0, offset_y=0)
    
    components = {}
    fonts = {}
    
    # Parse components
    comp_path = Path(components_dir)
    if comp_path.exists():
        for svg_file in sorted(comp_path.glob('*.svg')):
            if svg_file.name == 'Library.svg':
                continue
            
            name = svg_file.stem
            try:
                commands = parser.parse_file(str(svg_file))
                if commands:
                    components[name] = commands
                    print(f"✓ Component {name}: {len(commands)} commands")
                else:
                    print(f"✗ Component {name}: No commands generated")
            except Exception as e:
                print(f"✗ Component {name}: {e}")
    
    # Parse fonts
    font_path = Path(fonts_dir)
    if font_path.exists():
        for svg_file in sorted(font_path.glob('*.svg')):
            char = svg_file.stem
            try:
                commands = parser.parse_file(str(svg_file))
                if commands:
                    fonts[char] = commands
                    print(f"✓ Font '{char}': {len(commands)} commands")
                else:
                    print(f"✗ Font '{char}': No commands generated")
            except Exception as e:
                print(f"✗ Font '{char}': {e}")
    
    # Generate header file
    with open(output_file, 'w') as f:
        f.write(f"""// Auto-generated component and font library
// Generated from SVG assets - DO NOT EDIT MANUALLY
// Created by svg_to_lamp_final.py
//
// COORDINATES: Stored in PIXELS at {PIXELS_PER_MM} pixels/mm scale
// - reMarkable 2: {SCREEN_WIDTH}x{SCREEN_HEIGHT} pixels
// - render_component() applies position offset
//
// LAMP COMMAND SYNTAX:
//   pen down X Y        - Start drawing at (X,Y)
//   pen move X Y        - Draw line to (X,Y)
//   pen up              - Stop drawing
//   pen circle X Y R1 R2 - Draw circle at (X,Y) with radii R1,R2

#ifndef ELXNK_LIBRARY_H
#define ELXNK_LIBRARY_H

#include <string>
#include <cstring>

namespace elxnk {{

struct LampCommand {{
    const char* cmd;
}};

struct Component {{
    const char* name;
    const LampCommand* commands;
    int count;
}};

struct FontGlyph {{
    char character;
    const LampCommand* commands;
    int count;
}};

""")
        
        # Component arrays
        for name, commands in components.items():
            f.write(f"// Component: {name}\n")
            f.write(f"static const LampCommand {name}_commands[] = {{\n")
            for cmd in commands:
                f.write(f'    {{"{cmd}"}},\n')
            f.write("};\n\n")
        
        # Font arrays
        for char, commands in fonts.items():
            safe_name = char if char.isalnum() else f"_{ord(char)}"
            f.write(f"// Font: {char}\n")
            f.write(f"static const LampCommand font_{safe_name}_commands[] = {{\n")
            for cmd in commands:
                f.write(f'    {{"{cmd}"}},\n')
            f.write("};\n\n")
        
        # Lookup tables
        f.write("// Component lookup\n")
        f.write("static const Component COMPONENTS[] = {\n")
        for name, commands in components.items():
            f.write(f'    {{"{name}", {name}_commands, {len(commands)}}},\n')
        f.write("};\n\n")
        f.write(f"static const int COMPONENT_COUNT = {len(components)};\n\n")
        
        f.write("// Font glyph lookup\n")
        f.write("static const FontGlyph FONT_GLYPHS[] = {\n")
        for char, commands in fonts.items():
            safe_name = char if char.isalnum() else f"_{ord(char)}"
            f.write(f"    {{'{char}', font_{safe_name}_commands, {len(commands)}}},\n")
        f.write("};\n\n")
        f.write(f"static const int FONT_GLYPH_COUNT = {len(fonts)};\n\n")
        
        # Helper functions
        f.write("""// Helper functions
inline const Component* find_component(const char* name) {
    for (int i = 0; i < COMPONENT_COUNT; i++) {
        if (strcmp(COMPONENTS[i].name, name) == 0) {
            return &COMPONENTS[i];
        }
    }
    return nullptr;
}

inline const FontGlyph* find_glyph(char c) {
    for (int i = 0; i < FONT_GLYPH_COUNT; i++) {
        if (FONT_GLYPHS[i].character == c) {
            return &FONT_GLYPHS[i];
        }
    }
    return nullptr;
}

} // namespace elxnk

#endif // ELXNK_LIBRARY_H
""")
    
    total_commands = sum(len(c) for c in components.values()) + sum(len(f) for f in fonts.values())
    print(f"\n{'='*60}")
    print(f"Header file generated: {output_file}")
    print(f"  Components: {len(components)}")
    print(f"  Fonts: {len(fonts)}")
    print(f"  Total commands: {total_commands}")
    print(f"{'='*60}")


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        print("\nExamples:")
        print("  Single file:  python3 svg_to_lamp_final.py R.svg")
        print("  With params:  python3 svg_to_lamp_final.py R.svg 10 500 800")
        print("  Generate header: python3 svg_to_lamp_final.py --header assets/components assets/font src/component_library.h")
        sys.exit(1)
    
    # Header generation mode
    if sys.argv[1] == '--header':
        if len(sys.argv) < 5:
            print("Usage: --header <components_dir> <fonts_dir> <output.h>")
            sys.exit(1)
        
        if not HAS_SVGPATHTOOLS:
            print("ERROR: svgpathtools required for header generation")
            print("Install: pip install svgpathtools")
            sys.exit(1)
        
        generate_header_file(sys.argv[2], sys.argv[3], sys.argv[4])
        sys.exit(0)
    
    # Single file mode
    svg_file = sys.argv[1]
    scale = float(sys.argv[2]) if len(sys.argv) > 2 else None
    offset_x = int(sys.argv[3]) if len(sys.argv) > 3 else None
    offset_y = int(sys.argv[4]) if len(sys.argv) > 4 else None
    
    if not Path(svg_file).exists():
        print(f"Error: File not found: {svg_file}")
        sys.exit(1)
    
    parser = SVGToLamp(scale, offset_x, offset_y)
    commands = parser.parse_file(svg_file)
    
    # Output commands
    print(f"# Parsed {svg_file}", file=sys.stderr)
    print(f"# Scale: {parser.scale:.2f}, Offset: ({parser.offset_x}, {parser.offset_y})", file=sys.stderr)
    print(f"# Commands: {len(commands)}", file=sys.stderr)
    print()
    
    for cmd in commands:
        print(cmd)


if __name__ == '__main__':
    main()
