#!/usr/bin/env python3
"""
SVG to Lamp Command Converter - FINAL VERSION
Converts SVG files to lamp drawing commands for reMarkable 2
Uses rmkit-compatible geometry commands

LAMP GEOMETRY COMMANDS SUPPORTED (from updated lamp main.cpp):
  pen down X Y              - Start drawing at (X,Y)
  pen move X Y              - Draw line to (X,Y)
  pen up                    - Lift pen
  pen line X1 Y1 X2 Y2      - Draw line
  pen circle CX CY R1 R2    - Draw circle/ellipse
  pen rectangle X1 Y1 X2 Y2 - Draw rectangle
  pen arc CX CY R1 R2 A1 A2 - Draw arc
  pen bezier X1 Y1 ...      - Draw bezier curve
  fastpen ...               - Faster drawing (less sleep)

This script is called by svg2header.py to generate component library commands.
"""

import re
import sys
import xml.etree.ElementTree as ET
import math

# reMarkable 2 display constants
SCREEN_WIDTH = 1404
SCREEN_HEIGHT = 1872
PIXELS_PER_MM = 10.0  # Scaling factor for SVG mm to screen pixels


def line_distance(p1, p2):
    """Calculate Euclidean distance between two points"""
    return math.sqrt((p2[0] - p1[0])**2 + (p2[1] - p1[1])**2)


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


def simplify_path(points, tolerance=2.0):
    """
    Ramer-Douglas-Peucker algorithm to reduce path points
    Removes points that don't significantly change shape
    tolerance: maximum perpendicular distance in pixels
    """
    if len(points) < 3:
        return points

    # Find point with maximum distance from line start->end
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
        rec_results1 = simplify_path(points[:index+1], tolerance)
        rec_results2 = simplify_path(points[index:], tolerance)
        result = rec_results1[:-1] + rec_results2
    else:
        result = [points[0], points[end]]

    return result


def cubic_bezier_points(p0, p1, p2, p3, num_points=8):
    """
    Generate points along cubic bezier curve
    p0: start point, p1: control1, p2: control2, p3: end point
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


def parse_svg_to_commands(svg_path, use_fast=False):
    """
    Parse SVG file and generate lamp commands
    Returns list of command strings
    """
    try:
        tree = ET.parse(svg_path)
        root = tree.getroot()

        # Handle namespaces
        ns = {'svg': 'http://www.w3.org/2000/svg'}

        # Get transform from group if exists
        transform_x, transform_y = 0, 0
        groups = root.findall('.//svg:g', ns) or root.findall('.//g')
        if groups:
            transform = groups[0].get('transform', '')
            match = re.search(r'translate\(([^,]+),([^)]+)\)', transform)
            if match:
                transform_x = float(match.group(1))
                transform_y = float(match.group(2))

        commands = []
        pen_prefix = "fastpen" if use_fast else "pen"

        # Find all drawable elements using smart recursive search
        paths = root.findall('.//svg:path', ns) or root.findall('.//path')
        circles = root.findall('.//svg:circle', ns) or root.findall('.//circle')
        rects = root.findall('.//svg:rect', ns) or root.findall('.//rect')
        lines = root.findall('.//svg:line', ns) or root.findall('.//line')

        # Process paths (most common in components and fonts)
        for path_elem in paths:
            path_data = path_elem.get('d', '')
            if path_data:
                commands.extend(parse_path_to_commands(
                    path_data, transform_x, transform_y, pen_prefix
                ))

        # Process circles - USE GEOMETRY COMMAND (lamp now supports it!)
        for circle_elem in circles:
            cx = float(circle_elem.get('cx', 0))
            cy = float(circle_elem.get('cy', 0))
            r = float(circle_elem.get('r', 0))

            # Apply transform and convert to pixels
            cx_px = int(round((cx + transform_x) * PIXELS_PER_MM))
            cy_px = int(round((cy + transform_y) * PIXELS_PER_MM))
            r_px = int(round(r * PIXELS_PER_MM))

            # Use lamp's circle geometry command
            commands.append(f"{pen_prefix} circle {cx_px} {cy_px} {r_px} {r_px}")

        # Process rectangles - USE GEOMETRY COMMAND
        for rect_elem in rects:
            x = float(rect_elem.get('x', 0))
            y = float(rect_elem.get('y', 0))
            width = float(rect_elem.get('width', 0))
            height = float(rect_elem.get('height', 0))

            # Apply transform and convert to pixels
            x1 = int(round((x + transform_x) * PIXELS_PER_MM))
            y1 = int(round((y + transform_y) * PIXELS_PER_MM))
            x2 = int(round((x + width + transform_x) * PIXELS_PER_MM))
            y2 = int(round((y + height + transform_y) * PIXELS_PER_MM))

            # Use lamp's rectangle geometry command
            commands.append(f"{pen_prefix} rectangle {x1} {y1} {x2} {y2}")

        # Process lines - USE GEOMETRY COMMAND
        for line_elem in lines:
            x1 = float(line_elem.get('x1', 0))
            y1 = float(line_elem.get('y1', 0))
            x2 = float(line_elem.get('x2', 0))
            y2 = float(line_elem.get('y2', 0))

            # Apply transform and convert to pixels
            fx1 = int(round((x1 + transform_x) * PIXELS_PER_MM))
            fy1 = int(round((y1 + transform_y) * PIXELS_PER_MM))
            fx2 = int(round((x2 + transform_x) * PIXELS_PER_MM))
            fy2 = int(round((y2 + transform_y) * PIXELS_PER_MM))

            # Use lamp's line geometry command
            commands.append(f"{pen_prefix} line {fx1} {fy1} {fx2} {fy2}")

        return commands

    except Exception as e:
        print(f"Error parsing {svg_path}: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        return []


def parse_path_to_commands(path_data, tx, ty, pen_prefix):
    """
    Parse SVG path data into lamp commands
    Handles M, L, H, V, C, S, Z commands
    Applies path simplification for efficiency
    """
    commands = []

    # Tokenize path data
    tokens = re.findall(r'[MmLlHhVvCcSsZz]|[-+]?\d*\.?\d+(?:[eE][-+]?\d+)?', path_data)

    i = 0
    current_x, current_y = 0.0, 0.0
    start_x, start_y = 0.0, 0.0

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
                px = int(round((current_x + tx) * PIXELS_PER_MM))
                py = int(round((current_y + ty) * PIXELS_PER_MM))

                all_points = [(px, py)]

        elif cmd in 'Ll':  # Line
            j = 0
            while j + 1 < len(coords):
                if cmd == 'L':
                    current_x, current_y = coords[j], coords[j+1]
                else:
                    current_x += coords[j]
                    current_y += coords[j+1]

                px = int(round((current_x + tx) * PIXELS_PER_MM))
                py = int(round((current_y + ty) * PIXELS_PER_MM))
                all_points.append((px, py))
                j += 2

        elif cmd in 'Hh':  # Horizontal line
            for coord in coords:
                if cmd == 'H':
                    current_x = coord
                else:
                    current_x += coord

                px = int(round((current_x + tx) * PIXELS_PER_MM))
                py = int(round((current_y + ty) * PIXELS_PER_MM))
                all_points.append((px, py))

        elif cmd in 'Vv':  # Vertical line
            for coord in coords:
                if cmd == 'V':
                    current_y = coord
                else:
                    current_y += coord

                px = int(round((current_x + tx) * PIXELS_PER_MM))
                py = int(round((current_y + ty) * PIXELS_PER_MM))
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

                # Generate bezier curve points
                p0 = (current_x, current_y)
                p1 = (x1, y1)
                p2 = (x2, y2)
                p3 = (x3, y3)

                curve_pts = cubic_bezier_points(p0, p1, p2, p3, num_points=6)

                for pt_x, pt_y in curve_pts:
                    px = int(round((pt_x + tx) * PIXELS_PER_MM))
                    py = int(round((pt_y + ty) * PIXELS_PER_MM))
                    all_points.append((px, py))

                current_x, current_y = x3, y3
                j += 6

        elif cmd in 'Zz':  # Close path
            if current_x != start_x or current_y != start_y:
                px = int(round((start_x + tx) * PIXELS_PER_MM))
                py = int(round((start_y + ty) * PIXELS_PER_MM))
                all_points.append((px, py))
                current_x, current_y = start_x, start_y

    # Simplify collected points and generate commands
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
        print(f"Usage: {sys.argv[0]} <svg_file> [--fast]", file=sys.stderr)
        print(f"  Converts SVG to lamp drawing commands", file=sys.stderr)
        print(f"  --fast: Use fastpen commands for faster drawing", file=sys.stderr)
        sys.exit(1)

    svg_file = sys.argv[1]
    use_fast = '--fast' in sys.argv

    commands = parse_svg_to_commands(svg_file, use_fast)

    # Output commands to stdout
    for cmd in commands:
        print(cmd)
