#!/usr/bin/env python3
"""
SVG to Lamp Command Converter
Converts SVG files to embedded C header with lamp drawing commands
Pre-processes all components and fonts at build time

LAMP COMMAND SYNTAX:
  pen down X Y  - Put pen down at position (X,Y) and set current position
  pen move X Y  - Draw line from current position to (X,Y)
  pen up        - Lift pen
"""

import re
import sys
import os
from pathlib import Path
from typing import List
import xml.etree.ElementTree as ET


def parse_svg_to_lamp_commands(svg_path):
    """
    Parse SVG file and generate lamp commands with CORRECT syntax.
    Handles path, circle, rect, and line elements.
    """
    try:
        tree = ET.parse(svg_path)
        root = tree.getroot()

        # Handle namespaces
        ns = {'svg': 'http://www.w3.org/2000/svg'}

        # Get transform from group if exists
        groups = root.findall('.//svg:g', ns) or root.findall('.//g')
        transform_x, transform_y = 0, 0
        if groups:
            transform = groups[0].get('transform', '')
            match = re.search(r'translate\(([^,]+),([^)]+)\)', transform)
            if match:
                transform_x = float(match.group(1))
                transform_y = float(match.group(2))

        commands = []

        # Process all children in order (paths, circles, rects, lines)
        for group in groups:
            for element in group:
                tag = element.tag.split('}')[-1] if '}' in element.tag else element.tag

                if tag == 'path':
                    path_data = element.get('d', '')
                    if path_data:
                        commands.extend(parse_path_data(path_data, transform_x, transform_y))

                elif tag == 'circle':
                    commands.extend(parse_circle(element, transform_x, transform_y))

                elif tag == 'rect':
                    commands.extend(parse_rect(element, transform_x, transform_y))

                elif tag == 'line':
                    commands.extend(parse_line(element, transform_x, transform_y))

        if not commands:
            print(f"Warning: No drawable elements found in {svg_path}", file=sys.stderr)

        return commands

    except Exception as e:
        print(f"Error parsing {svg_path}: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        return []


def parse_circle(element, tx, ty):
    """Parse circle element and generate lamp circle command"""
    try:
        cx = float(element.get('cx', 0))
        cy = float(element.get('cy', 0))
        r = float(element.get('r', 0))

        # Apply transform and round properly (don't truncate small values)
        final_cx = int(round(cx + tx))
        final_cy = int(round(cy + ty))
        final_r = max(1, int(round(r)))  # Ensure minimum radius of 1 pixel

        # Lamp circle command: pen circle ox oy r1 r2
        # For a circle, r1 = r2 = radius
        return [f"pen circle {final_cx} {final_cy} {final_r} {final_r}"]

    except Exception as e:
        print(f"Error parsing circle: {e}", file=sys.stderr)
        return []


def parse_rect(element, tx, ty):
    """Parse rect element as 4 lines"""
    try:
        x = float(element.get('x', 0))
        y = float(element.get('y', 0))
        width = float(element.get('width', 0))
        height = float(element.get('height', 0))

        # Apply transform with rounding
        x1 = int(round(x + tx))
        y1 = int(round(y + ty))
        x2 = int(round(x + width + tx))
        y2 = int(round(y + height + ty))

        return [
            f"pen down {x1} {y1}",
            f"pen move {x2} {y1}",
            f"pen move {x2} {y2}",
            f"pen move {x1} {y2}",
            f"pen move {x1} {y1}",
            "pen up"
        ]

    except Exception as e:
        print(f"Error parsing rect: {e}", file=sys.stderr)
        return []


def parse_line(element, tx, ty):
    """Parse line element"""
    try:
        x1 = float(element.get('x1', 0))
        y1 = float(element.get('y1', 0))
        x2 = float(element.get('x2', 0))
        y2 = float(element.get('y2', 0))

        # Apply transform with rounding
        fx1 = int(round(x1 + tx))
        fy1 = int(round(y1 + ty))
        fx2 = int(round(x2 + tx))
        fy2 = int(round(y2 + ty))

        return [
            f"pen down {fx1} {fy1}",
            f"pen move {fx2} {fy2}",
            "pen up"
        ]

    except Exception as e:
        print(f"Error parsing line: {e}", file=sys.stderr)
        return []


def parse_path_data(path_data, tx, ty):
    """
    Parse SVG path data into CORRECT lamp commands.

    CRITICAL: lamp syntax is:
      - pen down X Y (to start drawing at X,Y)
      - pen move X Y (to draw line to X,Y)
      - pen up (to stop drawing)

    NOT:
      - pen move X Y + pen down (WRONG!)
    """
    commands = []

    # Tokenize
    tokens = re.findall(r'[MmLlHhVvCcSsQqTtAaZz]|[-+]?\d*\.?\d+(?:[eE][-+]?\d+)?', path_data)

    i = 0
    current_x, current_y = 0.0, 0.0
    start_x, start_y = 0.0, 0.0
    pen_is_down = False

    while i < len(tokens):
        if i >= len(tokens):
            break

        cmd = tokens[i]

        if not cmd[0].isalpha():
            i += 1
            continue

        # Collect coordinates
        coords = []
        i += 1
        while i < len(tokens) and not tokens[i][0].isalpha():
            try:
                coords.append(float(tokens[i]))
            except ValueError:
                break
            i += 1

        # Process command
        if cmd in 'Mm':  # Move
            if len(coords) >= 2:
                # Lift pen if down
                if pen_is_down:
                    commands.append("pen up")
                    pen_is_down = False

                # Update position
                if cmd == 'M':  # Absolute
                    current_x = coords[0]
                    current_y = coords[1]
                else:  # Relative
                    current_x += coords[0]
                    current_y += coords[1]

                start_x, start_y = current_x, current_y

                # Output with transform (use round for better accuracy)
                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))

                # CORRECT SYNTAX: pen down X Y (to start at this position)
                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

                # Process additional coordinates as implicit lineto
                j = 2
                while j + 1 < len(coords):
                    if cmd == 'M':
                        current_x = coords[j]
                        current_y = coords[j+1]
                    else:
                        current_x += coords[j]
                        current_y += coords[j+1]

                    final_x = int(round(current_x + tx))
                    final_y = int(round(current_y + ty))
                    commands.append(f"pen move {final_x} {final_y}")
                    j += 2

        elif cmd in 'Ll':  # Line
            if not pen_is_down:
                # If pen not down, we need to start at current position
                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))
                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

            j = 0
            while j + 1 < len(coords):
                if cmd == 'L':  # Absolute
                    current_x = coords[j]
                    current_y = coords[j+1]
                else:  # Relative
                    current_x += coords[j]
                    current_y += coords[j+1]

                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))
                commands.append(f"pen move {final_x} {final_y}")
                j += 2

        elif cmd in 'Hh':  # Horizontal line
            if not pen_is_down:
                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))
                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

            for coord in coords:
                if cmd == 'H':
                    current_x = coord
                else:
                    current_x += coord

                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))
                commands.append(f"pen move {final_x} {final_y}")

        elif cmd in 'Vv':  # Vertical line
            if not pen_is_down:
                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))
                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

            for coord in coords:
                if cmd == 'V':
                    current_y = coord
                else:
                    current_y += coord

                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))
                commands.append(f"pen move {final_x} {final_y}")

        elif cmd in 'Cc':  # Cubic Bezier
            if not pen_is_down:
                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))
                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

            j = 0
            while j + 5 < len(coords):
                if cmd == 'C':
                    current_x = coords[j+4]
                    current_y = coords[j+5]
                else:
                    current_x += coords[j+4]
                    current_y += coords[j+5]

                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))
                commands.append(f"pen move {final_x} {final_y}")
                j += 6

        elif cmd in 'Ss':  # Smooth cubic Bezier
            if not pen_is_down:
                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))
                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

            j = 0
            while j + 3 < len(coords):
                if cmd == 'S':
                    current_x = coords[j+2]
                    current_y = coords[j+3]
                else:
                    current_x += coords[j+2]
                    current_y += coords[j+3]

                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))
                commands.append(f"pen move {final_x} {final_y}")
                j += 4

        elif cmd in 'Zz':  # Close path
            if pen_is_down and (current_x != start_x or current_y != start_y):
                current_x, current_y = start_x, start_y
                final_x = int(round(current_x + tx))
                final_y = int(round(current_y + ty))
                commands.append(f"pen move {final_x} {final_y}")

    # Ensure pen is up at the end
    if pen_is_down:
        commands.append("pen up")

    return commands


def generate_header_file(components_dir: str, fonts_dir: str, output_file: str):
    """Generate C header file with embedded component and font data"""

    # Collect all components
    components = {}
    comp_path = Path(components_dir)
    if comp_path.exists():
        for svg_file in sorted(comp_path.glob('*.svg')):
            if svg_file.name != 'Library.svg':
                name = svg_file.stem
                commands = parse_svg_to_lamp_commands(str(svg_file))
                if commands:
                    components[name] = commands
                    print(f"Processed component: {name} ({len(commands)} commands)")

    # Collect all fonts
    fonts = {}
    font_path = Path(fonts_dir)
    if font_path.exists():
        for svg_file in sorted(font_path.glob('*.svg')):
            char = svg_file.stem
            commands = parse_svg_to_lamp_commands(str(svg_file))
            if commands:
                fonts[char] = commands
                print(f"Processed font: {char} ({len(commands)} commands)")

    # Generate header file
    with open(output_file, 'w') as f:
        f.write("""// Auto-generated component and font library
// Generated from SVG assets - DO NOT EDIT MANUALLY
// Build: make library (from src/ directory)
//
// LAMP COMMAND SYNTAX:
//   pen down X Y        - Put pen down at (X,Y), start drawing
//   pen move X Y        - Draw line from current position to (X,Y)
//   pen up              - Lift pen, stop drawing
//   pen circle X Y R1 R2 - Draw circle at (X,Y) with radii R1,R2

#ifndef ELXNK_LIBRARY_H
#define ELXNK_LIBRARY_H

#include <string>
#include <vector>
#include <map>

namespace elxnk {

// Lamp command structure
struct LampCommand {
    const char* cmd;
};

// Component definition
struct Component {
    const char* name;
    const LampCommand* commands;
    int count;
};

// Font glyph definition
struct FontGlyph {
    char character;
    const LampCommand* commands;
    int count;
};

""")

        # Write component command arrays
        for name, commands in components.items():
            f.write(f"// Component: {name}\n")
            f.write(f"static const LampCommand {name}_commands[] = {{\n")
            for cmd in commands:
                f.write(f'    {{"{cmd}"}},\n')
            f.write("};\n\n")

        # Write font command arrays
        for char, commands in fonts.items():
            safe_name = char if char.isalnum() else f"_{ord(char)}"
            f.write(f"// Font: {char}\n")
            f.write(f"static const LampCommand font_{safe_name}_commands[] = {{\n")
            for cmd in commands:
                f.write(f'    {{"{cmd}"}},\n')
            f.write("};\n\n")

        # Write component table
        f.write("// Component lookup table\n")
        f.write("static const Component COMPONENTS[] = {\n")
        for name, commands in components.items():
            f.write(f'    {{"{name}", {name}_commands, {len(commands)}}},\n')
        f.write("};\n\n")
        f.write(f"static const int COMPONENT_COUNT = {len(components)};\n\n")

        # Write font table
        f.write("// Font glyph lookup table\n")
        f.write("static const FontGlyph FONT_GLYPHS[] = {\n")
        for char, commands in fonts.items():
            safe_name = char if char.isalnum() else f"_{ord(char)}"
            f.write(f"    {{'{char}', font_{safe_name}_commands, {len(commands)}}},\n")
        f.write("};\n\n")
        f.write(f"static const int FONT_GLYPH_COUNT = {len(fonts)};\n\n")

        # Write helper functions
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

    print(f"\nGenerated header: {output_file}")
    print(f"  Components: {len(components)}")
    print(f"  Fonts: {len(fonts)}")
    print(f"  Total commands: {sum(len(c) for c in components.values()) + sum(len(f) for f in fonts.values())}")


if __name__ == '__main__':
    if len(sys.argv) < 4:
        print(f"Usage: {sys.argv[0]} <components_dir> <fonts_dir> <output.h>")
        print(f"Example: {sys.argv[0]} assets/components assets/font src/elxnk/component_library.h")
        sys.exit(1)

    generate_header_file(sys.argv[1], sys.argv[2], sys.argv[3])
