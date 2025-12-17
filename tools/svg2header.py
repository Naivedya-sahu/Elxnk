#!/usr/bin/env python3
"""
SVG to Lamp Command Converter
Converts SVG files to embedded C header with lamp drawing commands
Pre-processes all components and fonts at build time
"""

import re
import sys
import os
from pathlib import Path
from typing import List, Tuple, Dict
import xml.etree.ElementTree as ET


class SVGToLampConverter:
    """Converts SVG path data to lamp pen commands"""

    def __init__(self):
        self.commands = []

    def parse_svg_file(self, svg_path: str) -> List[str]:
        """Parse SVG file and extract lamp commands"""
        try:
            tree = ET.parse(svg_path)
            root = tree.getroot()

            # Handle namespaces
            ns = {'svg': 'http://www.w3.org/2000/svg'}
            paths = root.findall('.//svg:path', ns) or root.findall('.//path')

            if not paths:
                print(f"Warning: No paths found in {svg_path}", file=sys.stderr)
                return []

            all_commands = []
            for path in paths:
                d = path.get('d', '')
                if d:
                    all_commands.extend(self.parse_path_data(d))

            return all_commands

        except Exception as e:
            print(f"Error parsing {svg_path}: {e}", file=sys.stderr)
            return []

    def parse_path_data(self, path_data: str) -> List[str]:
        """Parse SVG path data into lamp commands"""
        commands = []

        # Simple tokenizer for SVG path commands
        # Handles: M/m (moveto), L/l (lineto), H/h (horizontal), V/v (vertical), C/c (cubic bezier)
        tokens = re.findall(r'[MmLlHhVvCcSsQqTtAaZz]|[-+]?\d*\.?\d+', path_data)

        i = 0
        current_x, current_y = 0, 0
        pen_down = False

        while i < len(tokens):
            cmd = tokens[i]

            if cmd in 'MmLlHhVvCcSsQqTtAa':
                # Get coordinates for this command
                coords = []
                i += 1
                while i < len(tokens) and tokens[i] not in 'MmLlHhVvCcSsQqTtAaZz':
                    try:
                        coords.append(float(tokens[i]))
                    except ValueError:
                        break
                    i += 1

                if cmd in 'Mm':  # Move
                    if len(coords) >= 2:
                        if pen_down:
                            commands.append("pen up")
                            pen_down = False

                        if cmd == 'M':  # Absolute
                            current_x, current_y = coords[0], coords[1]
                        else:  # Relative
                            current_x += coords[0]
                            current_y += coords[1]

                        commands.append(f"pen move {int(current_x)} {int(current_y)}")
                        commands.append("pen down")
                        pen_down = True

                elif cmd in 'Ll':  # Line
                    if not pen_down:
                        commands.append("pen down")
                        pen_down = True

                    j = 0
                    while j + 1 < len(coords):
                        if cmd == 'L':  # Absolute
                            current_x, current_y = coords[j], coords[j+1]
                        else:  # Relative
                            current_x += coords[j]
                            current_y += coords[j+1]
                        commands.append(f"pen move {int(current_x)} {int(current_y)}")
                        j += 2

                elif cmd in 'Hh':  # Horizontal line
                    if not pen_down:
                        commands.append("pen down")
                        pen_down = True

                    if cmd == 'H':
                        current_x = coords[0]
                    else:
                        current_x += coords[0]
                    commands.append(f"pen move {int(current_x)} {int(current_y)}")

                elif cmd in 'Vv':  # Vertical line
                    if not pen_down:
                        commands.append("pen down")
                        pen_down = True

                    if cmd == 'V':
                        current_y = coords[0]
                    else:
                        current_y += coords[0]
                    commands.append(f"pen move {int(current_x)} {int(current_y)}")

                elif cmd in 'Cc':  # Cubic Bezier - approximate with endpoint
                    if not pen_down:
                        commands.append("pen down")
                        pen_down = True

                    if len(coords) >= 6:
                        # Take endpoint (last 2 coords)
                        if cmd == 'C':
                            current_x, current_y = coords[-2], coords[-1]
                        else:
                            current_x += coords[-2]
                            current_y += coords[-1]
                        commands.append(f"pen move {int(current_x)} {int(current_y)}")

                elif cmd == 'Z' or cmd == 'z':  # Close path
                    i += 1
                    # Path will be closed automatically by next move

                else:
                    # Unsupported command, skip
                    i += 1
            else:
                i += 1

        if pen_down:
            commands.append("pen up")

        return commands


def generate_header_file(components_dir: str, fonts_dir: str, output_file: str):
    """Generate C header file with embedded component and font data"""

    converter = SVGToLampConverter()

    # Collect all components
    components = {}
    comp_path = Path(components_dir)
    if comp_path.exists():
        for svg_file in sorted(comp_path.glob('*.svg')):
            if svg_file.name != 'Library.svg':  # Skip library file
                name = svg_file.stem
                commands = converter.parse_svg_file(str(svg_file))
                if commands:
                    components[name] = commands
                    print(f"Processed component: {name} ({len(commands)} commands)")

    # Collect all fonts
    fonts = {}
    font_path = Path(fonts_dir)
    if font_path.exists():
        for svg_file in sorted(font_path.glob('*.svg')):
            # Extract character from filename (e.g., "A.svg" -> "A", "0.svg" -> "0")
            char = svg_file.stem
            commands = converter.parse_svg_file(str(svg_file))
            if commands:
                fonts[char] = commands
                print(f"Processed font: {char} ({len(commands)} commands)")

    # Generate header file
    with open(output_file, 'w') as f:
        f.write("""// Auto-generated component and font library
// Generated from SVG assets - DO NOT EDIT MANUALLY
// Build: make generate-library

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

// Send component to lamp with offset and scale
inline void render_component(const char* name, int offset_x, int offset_y, float scale, int lamp_fd) {
    const Component* comp = find_component(name);
    if (!comp) return;

    for (int i = 0; i < comp->count; i++) {
        std::string cmd = comp->commands[i].cmd;

        // Parse and transform coordinates
        if (cmd.find("pen move") == 0) {
            int x, y;
            if (sscanf(cmd.c_str(), "pen move %d %d", &x, &y) == 2) {
                x = (int)(x * scale) + offset_x;
                y = (int)(y * scale) + offset_y;
                cmd = "pen move " + std::to_string(x) + " " + std::to_string(y);
            }
        }

        cmd += "\\n";
        write(lamp_fd, cmd.c_str(), cmd.length());
    }
}

// Render text with fonts
inline void render_text(const char* text, int x, int y, float scale, int spacing, int lamp_fd) {
    int offset_x = x;

    for (const char* p = text; *p; p++) {
        char c = toupper(*p);  // Convert to uppercase

        if (c == ' ') {
            offset_x += spacing;
            continue;
        }

        const FontGlyph* glyph = find_glyph(c);
        if (!glyph) continue;

        for (int i = 0; i < glyph->count; i++) {
            std::string cmd = glyph->commands[i].cmd;

            if (cmd.find("pen move") == 0) {
                int gx, gy;
                if (sscanf(cmd.c_str(), "pen move %d %d", &gx, &gy) == 2) {
                    gx = (int)(gx * scale) + offset_x;
                    gy = (int)(gy * scale) + y;
                    cmd = "pen move " + std::to_string(gx) + " " + std::to_string(gy);
                }
            }

            cmd += "\\n";
            write(lamp_fd, cmd.c_str(), cmd.length());
        }

        offset_x += (int)(25 * scale);  // Character spacing
    }
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
