#!/usr/bin/env python3
"""
SVG to Lamp Command Converter - Complete SVG Support
Handles path, circle, and other SVG elements

LAMP COMMANDS:
  pen down X Y    - Start drawing at absolute pixel (X,Y)
  pen move X Y    - Draw line to absolute pixel (X,Y)
  pen up          - Lift pen
  pen circle X Y R1 R2 - Draw circle at (X,Y) with radii R1,R2

USAGE:
  python3 svg_to_lamp_complete.py <svg_file> [offset_x] [offset_y] [scale]
"""

import re
import sys
import math
import xml.etree.ElementTree as ET
from pathlib import Path


def parse_svg_file(svg_path, offset_x=0, offset_y=0, scale=1.0):
    """Parse SVG file and generate lamp commands"""
    try:
        tree = ET.parse(svg_path)
        root = tree.getroot()

        # Handle namespaces
        ns = {'svg': 'http://www.w3.org/2000/svg'}

        # Get transform from group
        groups = root.findall('.//svg:g', ns) or root.findall('.//g')
        transform_x, transform_y = 0, 0
        if groups:
            transform = groups[0].get('transform', '')
            match = re.search(r'translate\(([^,]+),([^)]+)\)', transform)
            if match:
                transform_x = float(match.group(1))
                transform_y = float(match.group(2))

        commands = []

        # Process all children in order (paths, circles, etc.)
        for group in groups:
            for element in group:
                tag = element.tag.split('}')[-1] if '}' in element.tag else element.tag

                if tag == 'path':
                    path_data = element.get('d', '')
                    if path_data:
                        commands.extend(parse_path(path_data, transform_x, transform_y, offset_x, offset_y, scale))

                elif tag == 'circle':
                    commands.extend(parse_circle(element, transform_x, transform_y, offset_x, offset_y, scale))

                elif tag == 'rect':
                    commands.extend(parse_rect(element, transform_x, transform_y, offset_x, offset_y, scale))

                elif tag == 'line':
                    commands.extend(parse_line(element, transform_x, transform_y, offset_x, offset_y, scale))

        return commands

    except Exception as e:
        print(f"Error parsing {svg_path}: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        return []


def parse_circle(element, tx, ty, offset_x, offset_y, scale):
    """Parse circle element and generate lamp circle command"""
    try:
        cx = float(element.get('cx', 0))
        cy = float(element.get('cy', 0))
        r = float(element.get('r', 0))

        # Apply transform, scale, offset
        final_cx = int((cx + tx) * scale + offset_x)
        final_cy = int((cy + ty) * scale + offset_y)
        final_r = int(r * scale)

        # Lamp circle command: pen circle ox oy r1 r2
        # For a circle, r1 = r2 = radius
        return [f"pen circle {final_cx} {final_cy} {final_r} {final_r}"]

    except Exception as e:
        print(f"Error parsing circle: {e}", file=sys.stderr)
        return []


def parse_rect(element, tx, ty, offset_x, offset_y, scale):
    """Parse rect element as 4 lines"""
    try:
        x = float(element.get('x', 0))
        y = float(element.get('y', 0))
        width = float(element.get('width', 0))
        height = float(element.get('height', 0))

        # Apply transform, scale, offset
        x1 = int((x + tx) * scale + offset_x)
        y1 = int((y + ty) * scale + offset_y)
        x2 = int((x + width + tx) * scale + offset_x)
        y2 = int((y + height + ty) * scale + offset_y)

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


def parse_line(element, tx, ty, offset_x, offset_y, scale):
    """Parse line element"""
    try:
        x1 = float(element.get('x1', 0))
        y1 = float(element.get('y1', 0))
        x2 = float(element.get('x2', 0))
        y2 = float(element.get('y2', 0))

        # Apply transform, scale, offset
        fx1 = int((x1 + tx) * scale + offset_x)
        fy1 = int((y1 + ty) * scale + offset_y)
        fx2 = int((x2 + tx) * scale + offset_x)
        fy2 = int((y2 + ty) * scale + offset_y)

        return [
            f"pen down {fx1} {fy1}",
            f"pen move {fx2} {fy2}",
            "pen up"
        ]

    except Exception as e:
        print(f"Error parsing line: {e}", file=sys.stderr)
        return []


def parse_path(path_data, tx, ty, offset_x, offset_y, scale):
    """Parse SVG path data"""
    commands = []
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

        coords = []
        i += 1
        while i < len(tokens) and not tokens[i][0].isalpha():
            try:
                coords.append(float(tokens[i]))
            except ValueError:
                break
            i += 1

        if cmd in 'Mm':  # Move
            if len(coords) >= 2:
                if pen_is_down:
                    commands.append("pen up")
                    pen_is_down = False

                if cmd == 'M':
                    current_x = coords[0]
                    current_y = coords[1]
                else:
                    current_x += coords[0]
                    current_y += coords[1]

                start_x, start_y = current_x, current_y

                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)

                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

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

        elif cmd in 'Ll':
            if not pen_is_down:
                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen down {final_x} {final_y}")
                pen_is_down = True

            j = 0
            while j + 1 < len(coords):
                if cmd == 'L':
                    current_x = coords[j]
                    current_y = coords[j+1]
                else:
                    current_x += coords[j]
                    current_y += coords[j+1]

                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")
                j += 2

        elif cmd in 'Hh':
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

        elif cmd in 'Vv':
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

        elif cmd in 'Cc':
            if not pen_is_down:
                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
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

                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")
                j += 6

        elif cmd in 'Ss':
            if not pen_is_down:
                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
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

                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")
                j += 4

        elif cmd in 'Zz':
            if pen_is_down and (current_x != start_x or current_y != start_y):
                current_x, current_y = start_x, start_y
                final_x = int((current_x + tx) * scale + offset_x)
                final_y = int((current_y + ty) * scale + offset_y)
                commands.append(f"pen move {final_x} {final_y}")

    if pen_is_down:
        commands.append("pen up")

    return commands


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <svg_file> [offset_x] [offset_y] [scale]", file=sys.stderr)
        print(f"Example: {sys.argv[0]} VDC.svg 500 500 2.0", file=sys.stderr)
        sys.exit(1)

    svg_file = sys.argv[1]
    offset_x = float(sys.argv[2]) if len(sys.argv) > 2 else 0
    offset_y = float(sys.argv[3]) if len(sys.argv) > 3 else 0
    scale = float(sys.argv[4]) if len(sys.argv) > 4 else 1.0

    if not Path(svg_file).exists():
        print(f"Error: File {svg_file} not found", file=sys.stderr)
        sys.exit(1)

    commands = parse_svg_file(svg_file, offset_x, offset_y, scale)

    for cmd in commands:
        print(cmd)
