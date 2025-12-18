#!/usr/bin/env python3
"""
SVG to Lamp Command Converter - Smart V2
Standalone script to convert individual SVG files to lamp commands

IMPORTANT: Lamp commands use ABSOLUTE pixel coordinates (1404x1872 screen)
  - offset_x, offset_y: Absolute screen position
  - scale: Scaling factor for component size
  - Output: pen down/move commands with absolute pixel coordinates

Usage:
    python3 svg_to_lamp_smartv2.py R.svg [offset_x] [offset_y] [scale]

Example:
    python3 svg_to_lamp_smartv2.py R.svg 500 500 2.0
    # Outputs commands like: pen down 502 502 (absolute screen pixels)

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

                # Calculate new position in UNTRANSFORMED space
                if cmd == 'M':  # Absolute
                    current_x = coords[0]
                    current_y = coords[1]
                else:  # Relative
                    current_x += coords[0]
                    current_y += coords[1]

                start_x, start_y = current_x, current_y

                # Apply transform, scale, then offset (absolute screen pixels)
                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)

                # For first move, add pen up first
                if is_first and not commands:
                    commands.append("pen up")

                # CORRECT SYNTAX: pen down X Y (starts drawing at absolute screen position)
                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

                # Process additional coordinate pairs as implicit lineto
                j = 2
                while j + 1 < len(coords):
                    if cmd == 'M':
                        current_x = coords[j]
                        current_y = coords[j+1]
                    else:
                        current_x += coords[j]
                        current_y += coords[j+1]

                    final_x = int((current_x + tx) * scale + offset_x)
                    final_y = int((current_y + ty) * scale + offset_y)
                    commands.append(f"pen move {final_x} {final_y}")
                    j += 2

        elif cmd in 'Ll':  # Line
            if not pen_is_down:
                # Start drawing at current position
                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
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

                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")
                j += 2

        elif cmd in 'Hh':  # Horizontal line
            if not pen_is_down:
                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

            for coord in coords:
                if cmd == 'H':
                    current_x = coord
                else:
                    current_x += coord

                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")

        elif cmd in 'Vv':  # Vertical line
            if not pen_is_down:
                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

            for coord in coords:
                if cmd == 'V':
                    current_y = coord
                else:
                    current_y += coord

                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")

        elif cmd in 'Cc':  # Cubic Bezier
            if not pen_is_down:
                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

            # Process in groups of 6 coordinates (x1,y1, x2,y2, x,y)
            j = 0
            while j + 5 < len(coords):
                # We approximate curve by just using the end point
                if cmd == 'C':
                    current_x = coords[j+4]
                    current_y = coords[j+5]
                else:
                    current_x += coords[j+4]
                    current_y += coords[j+5]

                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")
                j += 6

        elif cmd in 'Zz':  # Close path
            if pen_is_down and (current_x != start_x or current_y != start_y):
                # Draw line back to start
                current_x, current_y = start_x, start_y
                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
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
