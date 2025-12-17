#!/usr/bin/env python3
"""
SVG to Lamp Command Converter - Smart V2
Standalone script to convert individual SVG files to lamp commands
Based on original svg2lamp.sh logic

Usage:
    python3 svg_to_lamp_smartv2.py R.svg [offset_x] [offset_y] [scale]

Output: Lamp commands to stdout (one per line)
"""

import re
import sys
import xml.etree.ElementTree as ET
from pathlib import Path


def parse_svg_to_lamp_commands(svg_path, offset_x=0, offset_y=0, scale=1.0):
    """
    Parse SVG file and generate lamp commands.

    Returns list of lamp command strings.
    """
    try:
        tree = ET.parse(svg_path)
        root = tree.getroot()

        # Handle namespaces
        ns = {'svg': 'http://www.w3.org/2000/svg'}

        # Get SVG viewBox for coordinate normalization
        viewbox = root.get('viewBox')
        if viewbox:
            vb_parts = viewbox.split()
            vb_x, vb_y = float(vb_parts[0]), float(vb_parts[1])
        else:
            vb_x, vb_y = 0, 0

        # Get transform from group if exists
        groups = root.findall('.//svg:g', ns) or root.findall('.//g')
        transform_x, transform_y = 0, 0
        if groups:
            transform = groups[0].get('transform', '')
            match = re.search(r'translate\(([^,]+),([^)]+)\)', transform)
            if match:
                transform_x = float(match.group(1))
                transform_y = float(match.group(2))

        # Find all path elements
        paths = root.findall('.//svg:path', ns) or root.findall('.//path')

        if not paths:
            print(f"Warning: No paths found in {svg_path}", file=sys.stderr)
            return []

        commands = []
        first_path = True

        for path in paths:
            path_data = path.get('d', '')
            if not path_data:
                continue

            # Parse path data
            path_commands = parse_path_data(
                path_data,
                transform_x,
                transform_y,
                offset_x,
                offset_y,
                scale,
                is_first=first_path
            )
            commands.extend(path_commands)
            first_path = False

        return commands

    except Exception as e:
        print(f"Error parsing {svg_path}: {e}", file=sys.stderr)
        return []


def parse_path_data(path_data, tx, ty, offset_x, offset_y, scale, is_first=False):
    """
    Parse SVG path data string into lamp commands.

    tx, ty: transform translation from SVG group
    offset_x, offset_y: additional offset for positioning on screen
    scale: scaling factor
    is_first: whether this is the first path (affects pen up placement)
    """
    commands = []

    # Tokenize: extract commands (M,L,H,V,C,Z etc) and numbers
    tokens = re.findall(r'[MmLlHhVvCcSsQqTtAaZz]|[-+]?\d*\.?\d+(?:[eE][-+]?\d+)?', path_data)

    i = 0
    current_x, current_y = 0.0, 0.0
    start_x, start_y = 0.0, 0.0
    pen_is_down = False

    while i < len(tokens):
        if i >= len(tokens):
            break

        cmd = tokens[i]

        # Skip if not a command
        if not cmd[0].isalpha():
            i += 1
            continue

        # Collect coordinate parameters
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

                # Calculate new position
                if cmd == 'M':  # Absolute
                    new_x = coords[0] + tx
                    new_y = coords[1] + ty
                else:  # Relative
                    new_x = current_x + coords[0]
                    new_y = current_y + coords[1]

                current_x, current_y = new_x, new_y
                start_x, start_y = new_x, new_y

                # Transform and scale
                final_x = int(current_x * scale + offset_x)
                final_y = int(current_y * scale + offset_y)

                # For first move, add pen up first
                if is_first and not commands:
                    commands.append("pen up")

                commands.append(f"pen move {final_x} {final_y}")

                # Process additional coordinate pairs as implicit lineto
                j = 2
                while j + 1 < len(coords):
                    if not pen_is_down:
                        commands.append("pen down")
                        pen_is_down = True

                    if cmd == 'M':
                        new_x = coords[j] + tx
                        new_y = coords[j+1] + ty
                    else:
                        new_x = current_x + coords[j]
                        new_y = current_y + coords[j+1]

                    current_x, current_y = new_x, new_y
                    final_x = int(current_x * scale + offset_x)
                    final_y = int(current_y * scale + offset_y)
                    commands.append(f"pen move {final_x} {final_y}")
                    j += 2

        elif cmd in 'Ll':  # Line
            if not pen_is_down:
                commands.append("pen down")
                pen_is_down = True

            j = 0
            while j + 1 < len(coords):
                if cmd == 'L':  # Absolute
                    new_x = coords[j] + tx
                    new_y = coords[j+1] + ty
                else:  # Relative
                    new_x = current_x + coords[j]
                    new_y = current_y + coords[j+1]

                current_x, current_y = new_x, new_y
                final_x = int(current_x * scale + offset_x)
                final_y = int(current_y * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")
                j += 2

        elif cmd in 'Hh':  # Horizontal line
            if not pen_is_down:
                commands.append("pen down")
                pen_is_down = True

            for coord in coords:
                if cmd == 'H':
                    new_x = coord + tx
                else:
                    new_x = current_x + coord

                current_x = new_x
                final_x = int(current_x * scale + offset_x)
                final_y = int(current_y * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")

        elif cmd in 'Vv':  # Vertical line
            if not pen_is_down:
                commands.append("pen down")
                pen_is_down = True

            for coord in coords:
                if cmd == 'V':
                    new_y = coord + ty
                else:
                    new_y = current_y + coord

                current_y = new_y
                final_x = int(current_x * scale + offset_x)
                final_y = int(current_y * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")

        elif cmd in 'Cc':  # Cubic Bezier
            if not pen_is_down:
                commands.append("pen down")
                pen_is_down = True

            # Process in groups of 6 coordinates (x1,y1, x2,y2, x,y)
            j = 0
            while j + 5 < len(coords):
                # We approximate curve by just using the end point
                if cmd == 'C':
                    new_x = coords[j+4] + tx
                    new_y = coords[j+5] + ty
                else:
                    new_x = current_x + coords[j+4]
                    new_y = current_y + coords[j+5]

                current_x, current_y = new_x, new_y
                final_x = int(current_x * scale + offset_x)
                final_y = int(current_y * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")
                j += 6

        elif cmd in 'Zz':  # Close path
            if pen_is_down and (current_x != start_x or current_y != start_y):
                # Draw line back to start
                current_x, current_y = start_x, start_y
                final_x = int(current_x * scale + offset_x)
                final_y = int(current_y * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")

    # Ensure pen is up at the end
    if pen_is_down:
        commands.append("pen up")

    return commands


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <svg_file> [offset_x] [offset_y] [scale]", file=sys.stderr)
        print(f"Example: {sys.argv[0]} R.svg 500 500 1.0", file=sys.stderr)
        sys.exit(1)

    svg_file = sys.argv[1]
    offset_x = float(sys.argv[2]) if len(sys.argv) > 2 else 0
    offset_y = float(sys.argv[3]) if len(sys.argv) > 3 else 0
    scale = float(sys.argv[4]) if len(sys.argv) > 4 else 1.0

    if not Path(svg_file).exists():
        print(f"Error: File {svg_file} not found", file=sys.stderr)
        sys.exit(1)

    commands = parse_svg_to_lamp_commands(svg_file, offset_x, offset_y, scale)

    for cmd in commands:
        print(cmd)
