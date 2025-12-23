#!/usr/bin/env python3
"""
JSON Component Renderer
Converts JSON component/font definitions to lamp drawing commands

Usage:
    render_json.py component <name> <x> <y> [scale]
    render_json.py text <text> <x> <y> [scale]
    render_json.py list

Examples:
    render_json.py component R 500 400 1.0
    render_json.py text "Hello" 300 200 1.5
    render_json.py list
"""

import json
import sys
import math
from pathlib import Path


def load_library():
    """Load component and font JSON libraries"""
    script_dir = Path(__file__).parent
    lib_dir = script_dir.parent / 'lib'

    with open(lib_dir / 'components.json', 'r') as f:
        components = json.load(f)

    with open(lib_dir / 'fonts.json', 'r') as f:
        fonts = json.load(f)

    return components, fonts


def relative_to_absolute(rel_x, rel_y, bbox_width, bbox_height, offset_x, offset_y, scale):
    """Convert relative coordinates (0-1) to absolute screen pixels"""
    abs_x = int(round(offset_x + (rel_x * bbox_width * scale)))
    abs_y = int(round(offset_y + (rel_y * bbox_height * scale)))
    return abs_x, abs_y


def render_path(path, bbox_width, bbox_height, offset_x, offset_y, scale):
    """Render a path from JSON to lamp commands"""
    commands = []

    if path['type'] == 'line':
        # Simple line
        x1, y1 = relative_to_absolute(
            path['from']['x'], path['from']['y'],
            bbox_width, bbox_height, offset_x, offset_y, scale
        )
        x2, y2 = relative_to_absolute(
            path['to']['x'], path['to']['y'],
            bbox_width, bbox_height, offset_x, offset_y, scale
        )
        commands.append(f"pen line {x1} {y1} {x2} {y2}")

    elif path['type'] == 'circle':
        # Circle
        cx, cy = relative_to_absolute(
            path['cx'], path['cy'],
            bbox_width, bbox_height, offset_x, offset_y, scale
        )
        r = int(round(path['r'] * min(bbox_width, bbox_height) * scale))
        commands.append(f"pen circle {cx} {cy} {r} {r}")

    elif path['type'] == 'path':
        # Complex path with move/line commands
        points = []
        for cmd_obj in path['commands']:
            cmd = cmd_obj['cmd']

            if cmd == 'M':  # Move to
                x, y = relative_to_absolute(
                    cmd_obj['x'], cmd_obj['y'],
                    bbox_width, bbox_height, offset_x, offset_y, scale
                )
                if points:
                    # Output previous path
                    if len(points) > 0:
                        commands.append(f"pen down {points[0][0]} {points[0][1]}")
                        for px, py in points[1:]:
                            commands.append(f"pen move {px} {py}")
                        commands.append("pen up")
                    points = []
                points.append((x, y))

            elif cmd == 'L':  # Line to
                x, y = relative_to_absolute(
                    cmd_obj['x'], cmd_obj['y'],
                    bbox_width, bbox_height, offset_x, offset_y, scale
                )
                points.append((x, y))

            elif cmd == 'C':  # Cubic bezier
                # For now, approximate with line to endpoint
                # Full bezier implementation can be added later
                x, y = relative_to_absolute(
                    cmd_obj['x'], cmd_obj['y'],
                    bbox_width, bbox_height, offset_x, offset_y, scale
                )
                points.append((x, y))

        # Output final path
        if points:
            commands.append(f"pen down {points[0][0]} {points[0][1]}")
            for px, py in points[1:]:
                commands.append(f"pen move {px} {py}")
            commands.append("pen up")

    return commands


def render_component(component_name, x, y, scale, components):
    """Render a component at specified position"""
    if component_name not in components['components']:
        print(f"Error: Component '{component_name}' not found", file=sys.stderr)
        return []

    comp = components['components'][component_name]
    bbox_width = comp['bbox']['width']
    bbox_height = comp['bbox']['height']

    commands = []
    for path in comp['paths']:
        commands.extend(render_path(path, bbox_width, bbox_height, x, y, scale))

    return commands


def render_text(text, x, y, scale, fonts):
    """Render text string at specified position"""
    commands = []
    cursor_x = x

    font_metadata = fonts['metadata']
    default_height = font_metadata['default_height']
    spacing = int(default_height * scale * 0.3)  # 30% spacing between chars

    for char in text.upper():
        if char == ' ':
            cursor_x += spacing * 2
            continue

        if char not in fonts['fonts']:
            print(f"Warning: Character '{char}' not found in font library", file=sys.stderr)
            cursor_x += spacing
            continue

        font_char = fonts['fonts'][char]
        bbox_width = font_char['bbox']['width']
        bbox_height = font_char['bbox']['height']

        for path in font_char['paths']:
            commands.extend(render_path(path, bbox_width, bbox_height, cursor_x, y, scale))

        cursor_x += int(bbox_width * scale) + spacing

    return commands


def list_components(components, fonts):
    """List available components and fonts"""
    print("Available Components:")
    for name, comp in sorted(components['components'].items()):
        comp_type = comp.get('type', 'unknown')
        comp_name = comp.get('name', name)
        bbox = comp['bbox']
        print(f"  {name:10s} - {comp_name:20s} ({comp_type:12s}) [{bbox['width']}x{bbox['height']}px]")

    print("\nAvailable Font Characters:")
    chars = sorted(fonts['fonts'].keys())
    print(f"  {', '.join(chars)}")


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    command = sys.argv[1]

    components, fonts = load_library()

    if command == 'list':
        list_components(components, fonts)
        return

    elif command == 'component':
        if len(sys.argv) < 5:
            print("Usage: render_json.py component <name> <x> <y> [scale]", file=sys.stderr)
            sys.exit(1)

        comp_name = sys.argv[2]
        x = int(sys.argv[3])
        y = int(sys.argv[4])
        scale = float(sys.argv[5]) if len(sys.argv) > 5 else 1.0

        commands = render_component(comp_name, x, y, scale, components)
        try:
            for cmd in commands:
                print(cmd)
        except BrokenPipeError:
            # Ignore broken pipe (e.g., when piping to head)
            sys.stderr.close()
            pass

    elif command == 'text':
        if len(sys.argv) < 5:
            print("Usage: render_json.py text <text> <x> <y> [scale]", file=sys.stderr)
            sys.exit(1)

        text = sys.argv[2]
        x = int(sys.argv[3])
        y = int(sys.argv[4])
        scale = float(sys.argv[5]) if len(sys.argv) > 5 else 1.0

        commands = render_text(text, x, y, scale, fonts)
        try:
            for cmd in commands:
                print(cmd)
        except BrokenPipeError:
            # Ignore broken pipe (e.g., when piping to head)
            sys.stderr.close()
            pass

    else:
        print(f"Unknown command: {command}", file=sys.stderr)
        print(__doc__)
        sys.exit(1)


if __name__ == '__main__':
    main()
