#!/usr/bin/env python3
"""
Fast SVG to Lamp Converter - Minimal Commands
Generates only essential pen commands for fast drawing

LAMP Commands Supported (from lamp main.cpp):
  pen down X Y    - Start drawing at (X,Y)
  pen move X Y    - Draw line to (X,Y)
  pen up          - Lift pen
  fastpen down/move/up - Faster drawing (less sleep)

NO circle command in lamp - must draw circles as line segments
"""

import re
import sys
import xml.etree.ElementTree as ET
import math

# reMarkable 2 display
SCREEN_WIDTH = 1404
SCREEN_HEIGHT = 1872
PIXELS_PER_MM = 10.0  # Scale factor


def line_distance(p1, p2):
    """Calculate distance between two points"""
    return math.sqrt((p2[0] - p1[0])**2 + (p2[1] - p1[1])**2)


def simplify_path(points, tolerance=2.0):
    """
    Ramer-Douglas-Peucker algorithm to reduce points
    Removes points that don't significantly change path shape
    """
    if len(points) < 3:
        return points

    # Find point with maximum distance from line
    dmax = 0
    index = 0
    end = len(points) - 1

    for i in range(1, end):
        d = point_line_distance(points[i], points[0], points[end])
        if d > dmax:
            index = i
            dmax = d

    # If max distance is greater than tolerance, recursively simplify
    if dmax > tolerance:
        # Recursive call
        rec_results1 = simplify_path(points[:index+1], tolerance)
        rec_results2 = simplify_path(points[index:], tolerance)

        # Build result list
        result = rec_results1[:-1] + rec_results2
    else:
        result = [points[0], points[end]]

    return result


def point_line_distance(point, line_start, line_end):
    """Calculate perpendicular distance from point to line"""
    x0, y0 = point
    x1, y1 = line_start
    x2, y2 = line_end

    numerator = abs((y2-y1)*x0 - (x2-x1)*y0 + x2*y1 - y2*x1)
    denominator = math.sqrt((y2-y1)**2 + (x2-x1)**2)

    if denominator == 0:
        return line_distance(point, line_start)

    return numerator / denominator


def cubic_bezier_points(p0, p1, p2, p3, num_points=8):
    """
    Generate points along cubic bezier curve
    Fewer points than before for speed (8 instead of 10)
    """
    points = []
    for i in range(num_points + 1):
        t = i / num_points
        t2 = t * t
        t3 = t2 * t
        mt = 1 - t
        mt2 = mt * mt
        mt3 = mt2 * mt

        x = mt3*p0[0] + 3*mt2*t*p1[0] + 3*mt*t2*p2[0] + t3*p3[0]
        y = mt3*p0[1] + 3*mt2*t*p1[1] + 3*mt*t2*p2[1] + t3*p3[1]

        points.append((x, y))

    return points


def circle_to_points(cx, cy, r, num_segments=16):
    """
    Convert circle to line segments
    16 segments for balance of speed and smoothness
    """
    points = []
    for i in range(num_segments + 1):
        angle = 2 * math.pi * i / num_segments
        x = cx + r * math.cos(angle)
        y = cy + r * math.sin(angle)
        points.append((x, y))

    return points


def parse_svg_fast(svg_path, offset_x=0, offset_y=0, scale=1.0, fast=False):
    """
    Fast SVG parser with minimal commands
    """
    try:
        tree = ET.parse(svg_path)
        root = tree.getroot()

        # Namespaces
        ns = {'svg': 'http://www.w3.org/2000/svg'}

        # Get transform
        transform_x, transform_y = 0, 0
        groups = root.findall('.//svg:g', ns) or root.findall('.//g')
        if groups:
            transform = groups[0].get('transform', '')
            match = re.search(r'translate\(([^,]+),([^)]+)\)', transform)
            if match:
                transform_x = float(match.group(1))
                transform_y = float(match.group(2))

        commands = []
        pen_prefix = "fastpen" if fast else "pen"

        # Process paths
        paths = root.findall('.//svg:path', ns) or root.findall('.//path')
        for path_elem in paths:
            path_data = path_elem.get('d', '')
            if path_data:
                commands.extend(parse_path_fast(
                    path_data, transform_x, transform_y,
                    offset_x, offset_y, scale, pen_prefix
                ))

        # Process circles
        circles = root.findall('.//svg:circle', ns) or root.findall('.//circle')
        for circle_elem in circles:
            cx = float(circle_elem.get('cx', 0))
            cy = float(circle_elem.get('cy', 0))
            r = float(circle_elem.get('r', 0))

            # Convert to screen coordinates
            cx_mm = cx + transform_x
            cy_mm = cy + transform_y
            r_mm = r

            cx_px = int(round((cx_mm * PIXELS_PER_MM * scale) + offset_x))
            cy_px = int(round((cy_mm * PIXELS_PER_MM * scale) + offset_y))
            r_px = int(round(r_mm * PIXELS_PER_MM * scale))

            # Generate circle as line segments
            points = circle_to_points(cx_px, cy_px, r_px, num_segments=16)

            if points:
                commands.append(f"{pen_prefix} down {int(points[0][0])} {int(points[0][1])}")
                for x, y in points[1:]:
                    commands.append(f"{pen_prefix} move {int(x)} {int(y)}")
                commands.append(f"{pen_prefix} up")

        return commands

    except Exception as e:
        print(f"Error parsing {svg_path}: {e}", file=sys.stderr)
        return []


def parse_path_fast(path_data, tx, ty, offset_x, offset_y, scale, pen_prefix):
    """
    Fast path parser with simplified curves
    """
    commands = []

    # Tokenize
    tokens = re.findall(r'[MmLlHhVvCcSsZz]|[-+]?\d*\.?\d+(?:[eE][-+]?\d+)?', path_data)

    i = 0
    current_x, current_y = 0.0, 0.0
    start_x, start_y = 0.0, 0.0
    pen_down = False

    all_points = []  # Collect points for simplification

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
                if cmd == 'M':
                    current_x, current_y = coords[0], coords[1]
                else:
                    current_x += coords[0]
                    current_y += coords[1]

                start_x, start_y = current_x, current_y

                # Convert to pixels
                px = int(round((current_x + tx) * PIXELS_PER_MM * scale + offset_x))
                py = int(round((current_y + ty) * PIXELS_PER_MM * scale + offset_y))

                if pen_down:
                    commands.append(f"{pen_prefix} up")

                all_points = [(px, py)]
                pen_down = True

        elif cmd in 'Ll':  # Line
            j = 0
            while j + 1 < len(coords):
                if cmd == 'L':
                    current_x, current_y = coords[j], coords[j+1]
                else:
                    current_x += coords[j]
                    current_y += coords[j+1]

                px = int(round((current_x + tx) * PIXELS_PER_MM * scale + offset_x))
                py = int(round((current_y + ty) * PIXELS_PER_MM * scale + offset_y))

                all_points.append((px, py))
                j += 2

        elif cmd in 'Hh':  # Horizontal
            for coord in coords:
                if cmd == 'H':
                    current_x = coord
                else:
                    current_x += coord

                px = int(round((current_x + tx) * PIXELS_PER_MM * scale + offset_x))
                py = int(round((current_y + ty) * PIXELS_PER_MM * scale + offset_y))

                all_points.append((px, py))

        elif cmd in 'Vv':  # Vertical
            for coord in coords:
                if cmd == 'V':
                    current_y = coord
                else:
                    current_y += coord

                px = int(round((current_x + tx) * PIXELS_PER_MM * scale + offset_x))
                py = int(round((current_y + ty) * PIXELS_PER_MM * scale + offset_y))

                all_points.append((px, py))

        elif cmd in 'Cc':  # Cubic bezier
            j = 0
            while j + 5 < len(coords):
                if cmd == 'C':
                    x1, y1 = coords[j], coords[j+1]
                    x2, y2 = coords[j+2], coords[j+3]
                    x3, y3 = coords[j+4], coords[j+5]
                else:
                    x1 = current_x + coords[j]
                    y1 = current_y + coords[j+1]
                    x2 = current_x + coords[j+2]
                    y2 = current_y + coords[j+3]
                    x3 = current_x + coords[j+4]
                    y3 = current_y + coords[j+5]

                # Bezier curve points
                p0 = (current_x, current_y)
                p1 = (x1, y1)
                p2 = (x2, y2)
                p3 = (x3, y3)

                curve_pts = cubic_bezier_points(p0, p1, p2, p3, num_points=6)

                for pt_x, pt_y in curve_pts:
                    px = int(round((pt_x + tx) * PIXELS_PER_MM * scale + offset_x))
                    py = int(round((pt_y + ty) * PIXELS_PER_MM * scale + offset_y))
                    all_points.append((px, py))

                current_x, current_y = x3, y3
                j += 6

        elif cmd in 'Zz':  # Close path
            if current_x != start_x or current_y != start_y:
                px = int(round((start_x + tx) * PIXELS_PER_MM * scale + offset_x))
                py = int(round((start_y + ty) * PIXELS_PER_MM * scale + offset_y))
                all_points.append((px, py))
                current_x, current_y = start_x, start_y

    # Simplify collected points and output commands
    if len(all_points) > 2:
        simplified = simplify_path(all_points, tolerance=2.0)

        if simplified:
            commands.append(f"{pen_prefix} down {simplified[0][0]} {simplified[0][1]}")
            for px, py in simplified[1:]:
                commands.append(f"{pen_prefix} move {px} {py}")
            commands.append(f"{pen_prefix} up")
    elif len(all_points) > 0:
        commands.append(f"{pen_prefix} down {all_points[0][0]} {all_points[0][1]}")
        for px, py in all_points[1:]:
            commands.append(f"{pen_prefix} move {px} {py}")
        commands.append(f"{pen_prefix} up")

    return commands


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <svg_file> [offset_x] [offset_y] [scale] [--fast]")
        print(f"Example: {sys.argv[0]} R.svg 700 900 1.0")
        print(f"         {sys.argv[0]} D.svg 500 500 2.0 --fast")
        sys.exit(1)

    svg_file = sys.argv[1]
    offset_x = int(sys.argv[2]) if len(sys.argv) > 2 else 0
    offset_y = int(sys.argv[3]) if len(sys.argv) > 3 else 0
    scale = float(sys.argv[4]) if len(sys.argv) > 4 else 1.0
    fast = '--fast' in sys.argv

    commands = parse_svg_fast(svg_file, offset_x, offset_y, scale, fast)

    for cmd in commands:
        print(cmd)
