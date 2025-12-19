#!/usr/bin/env python3
"""
SVG to C Header Generator
Generates component library header file from SVG assets
Calls svg_to_lamp_final.py for SVG parsing

This script orchestrates the build process:
1. Iterates through component and font SVG files
2. Calls svg_to_lamp_final.py to convert each SVG to lamp commands
3. Generates C header file with embedded command arrays
"""

import sys
import os
import subprocess
from pathlib import Path


def parse_svg_to_lamp_commands(svg_file):
    """
    Parse SVG file by calling svg_to_lamp_final.py
    Returns list of command strings
    """
    try:
        # Get path to svg_to_lamp_final.py (in same directory as this script)
        script_dir = Path(__file__).parent
        converter_script = script_dir / 'svg_to_lamp_final.py'

        if not converter_script.exists():
            print(f"Error: svg_to_lamp_final.py not found at {converter_script}", file=sys.stderr)
            return []

        # Call svg_to_lamp_final.py as subprocess
        result = subprocess.run(
            ['python3', str(converter_script), svg_file],
            capture_output=True,
            text=True,
            check=True
        )

        # Parse output commands (one per line)
        commands = [line.strip() for line in result.stdout.strip().split('\n') if line.strip()]
        return commands

    except subprocess.CalledProcessError as e:
        print(f"Error converting {svg_file}: {e}", file=sys.stderr)
        print(f"stderr: {e.stderr}", file=sys.stderr)
        return []
    except Exception as e:
        print(f"Error processing {svg_file}: {e}", file=sys.stderr)
        return []


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
// Generated from SVG assets using svg_to_lamp_final.py
// Build: make library (from src/ directory)
// DO NOT EDIT MANUALLY - regenerate with svg2header.py
//
// COORDINATES: Stored in PIXELS at 10 pixels/mm scale
// - reMarkable 2: 1404x1872 pixels
// - Components scaled to actual pixel sizes
// - render_component() applies POSITION OFFSET only
//
// LAMP GEOMETRY COMMANDS (from rmkit):
//   pen down X Y              - Start drawing at (X,Y)
//   pen move X Y              - Draw line to (X,Y)
//   pen up                    - Lift pen
//   pen line X1 Y1 X2 Y2      - Draw line
//   pen circle CX CY R1 R2    - Draw circle/ellipse
//   pen rectangle X1 Y1 X2 Y2 - Draw rectangle
//   pen arc CX CY R1 R2 A1 A2 - Draw arc

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

        # Write component arrays
        for name, cmds in sorted(components.items()):
            f.write(f"// Component: {name}\n")
            f.write(f"static const LampCommand {name}_commands[] = {{\n")
            for cmd in cmds:
                # Escape quotes in commands
                escaped_cmd = cmd.replace('"', '\\"')
                f.write(f'    {{"{escaped_cmd}"}},\n')
            f.write("};\n\n")

        # Write font glyph arrays
        for char, cmds in sorted(fonts.items()):
            safe_name = char.replace('-', '_').replace('+', 'plus')
            f.write(f"// Font: {char}\n")
            f.write(f"static const LampCommand font_{safe_name}_commands[] = {{\n")
            for cmd in cmds:
                escaped_cmd = cmd.replace('"', '\\"')
                f.write(f'    {{"{escaped_cmd}"}},\n')
            f.write("};\n\n")

        # Write component registry
        f.write("// Component registry\n")
        f.write("static const Component COMPONENTS[] = {\n")
        for name, cmds in sorted(components.items()):
            count = len(cmds)
            f.write(f'    {{"{name}", {name}_commands, {count}}},\n')
        f.write("};\n\n")

        # Write font registry
        f.write("// Font glyph registry\n")
        f.write("static const FontGlyph FONT_GLYPHS[] = {\n")
        for char, cmds in sorted(fonts.items()):
            safe_name = char.replace('-', '_').replace('+', 'plus')
            count = len(cmds)
            # Map font name to character
            if len(char) == 1:
                f.write(f"    {{'{char}', font_{safe_name}_commands, {count}}},\n")
            else:
                # For multi-character names (like A-F), use first char
                first_char = char[0] if char else '?'
                f.write(f"    {{'{first_char}', font_{safe_name}_commands, {count}}},\n")
        f.write("};\n\n")

        # Write lookup functions
        f.write("""// Lookup functions
const Component* find_component(const std::string& name) {
    for (const auto& comp : COMPONENTS) {
        if (comp.name == name) {
            return &comp;
        }
    }
    return nullptr;
}

const FontGlyph* find_glyph(char c) {
    for (const auto& glyph : FONT_GLYPHS) {
        if (glyph.character == c) {
            return &glyph;
        }
    }
    return nullptr;
}

// Component count
inline int get_component_count() {
    return sizeof(COMPONENTS) / sizeof(Component);
}

// Font glyph count
inline int get_glyph_count() {
    return sizeof(FONT_GLYPHS) / sizeof(FontGlyph);
}

}  // namespace elxnk

#endif  // ELXNK_LIBRARY_H
""")

    total_components = len(components)
    total_fonts = len(fonts)
    total_commands = sum(len(cmds) for cmds in components.values()) + sum(len(cmds) for cmds in fonts.values())

    print(f"\nLibrary generated successfully:")
    print(f"  Components: {total_components}")
    print(f"  Font glyphs: {total_fonts}")
    print(f"  Total commands: {total_commands}")
    print(f"  Output: {output_file}")


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <components_dir> <fonts_dir> <output_file>", file=sys.stderr)
        print(f"Example: {sys.argv[0]} ../assets/components ../assets/font elxnk/component_library.h", file=sys.stderr)
        sys.exit(1)

    components_dir = sys.argv[1]
    fonts_dir = sys.argv[2]
    output_file = sys.argv[3]

    generate_header_file(components_dir, fonts_dir, output_file)
