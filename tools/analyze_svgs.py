#!/usr/bin/env python3
"""
SVG Analysis Tool - Detailed inspection of SVG files
Shows what elements and commands exist in each file
Identifies potential parsing issues
"""

import sys
import re
from pathlib import Path
import xml.etree.ElementTree as ET

def analyze_svg(svg_path):
    """Analyze SVG file and report all elements"""
    tree = ET.parse(svg_path)
    root = tree.getroot()
    
    stats = {
        'paths': 0,
        'circles': 0,
        'rects': 0,
        'lines': 0,
        'polylines': 0,
        'polygons': 0,
        'groups': 0,
        'transforms': 0,
        'path_commands': set(),
        'has_negative_coords': False,
        'viewbox': None,
        'size': None
    }
    
    # Get viewBox
    viewbox = root.get('viewBox')
    if viewbox:
        stats['viewbox'] = viewbox
    
    # Get size
    width = root.get('width')
    height = root.get('height')
    if width and height:
        stats['size'] = f"{width} x {height}"
    
    # Analyze all elements
    for elem in root.iter():
        tag = elem.tag.split('}')[-1]
        
        if tag == 'path':
            stats['paths'] += 1
            d = elem.get('d', '')
            if d:
                # Extract command letters
                commands = set(re.findall(r'[MmLlHhVvCcSsQqTtAaZz]', d))
                stats['path_commands'].update(commands)
                
                # Check for negative coordinates
                if re.search(r'-\d', d):
                    stats['has_negative_coords'] = True
        
        elif tag == 'circle':
            stats['circles'] += 1
            cx = elem.get('cx', '0')
            if '-' in cx:
                stats['has_negative_coords'] = True
        
        elif tag == 'rect':
            stats['rects'] += 1
        
        elif tag == 'line':
            stats['lines'] += 1
        
        elif tag == 'polyline':
            stats['polylines'] += 1
        
        elif tag == 'polygon':
            stats['polygons'] += 1
        
        elif tag == 'g':
            stats['groups'] += 1
            if elem.get('transform'):
                stats['transforms'] += 1
    
    return stats

def print_report(name, stats):
    """Print analysis report for one file"""
    print(f"\n{name}")
    print("=" * len(name))
    
    if stats['viewbox']:
        print(f"ViewBox: {stats['viewbox']}")
    if stats['size']:
        print(f"Size: {stats['size']}")
    
    print(f"\nElements:")
    print(f"  Paths: {stats['paths']}")
    print(f"  Circles: {stats['circles']}")
    print(f"  Rectangles: {stats['rects']}")
    print(f"  Lines: {stats['lines']}")
    print(f"  Polylines: {stats['polylines']}")
    print(f"  Polygons: {stats['polygons']}")
    print(f"  Groups: {stats['groups']}")
    print(f"  Transforms: {stats['transforms']}")
    
    if stats['path_commands']:
        print(f"\nPath Commands: {' '.join(sorted(stats['path_commands']))}")
    
    if stats['has_negative_coords']:
        print("\n⚠ Contains negative coordinates")
    
    # Identify potential issues
    issues = []
    if 'C' in stats['path_commands'] or 'c' in stats['path_commands']:
        issues.append("Has cubic bezier curves (needs interpolation)")
    if 'Q' in stats['path_commands'] or 'q' in stats['path_commands']:
        issues.append("Has quadratic bezier curves")
    if 'A' in stats['path_commands'] or 'a' in stats['path_commands']:
        issues.append("Has arc commands")
    if stats['has_negative_coords']:
        issues.append("Has negative coordinates (needs proper transform handling)")
    if stats['transforms'] > 0:
        issues.append("Has group transforms (must be applied)")
    
    if issues:
        print("\n⚠ Parsing Considerations:")
        for issue in issues:
            print(f"  • {issue}")

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 analyze_svgs.py <directory_or_file>")
        print("\nExamples:")
        print("  python3 analyze_svgs.py assets/components/")
        print("  python3 analyze_svgs.py assets/components/R.svg")
        sys.exit(1)
    
    path = Path(sys.argv[1])
    
    if not path.exists():
        print(f"Error: Path not found: {path}")
        sys.exit(1)
    
    files = []
    if path.is_file():
        files = [path]
    elif path.is_dir():
        files = sorted(path.glob('*.svg'))
    
    if not files:
        print(f"No SVG files found in {path}")
        sys.exit(1)
    
    print(f"Analyzing {len(files)} SVG file(s)...")
    
    # Collect statistics
    all_stats = {}
    for svg_file in files:
        try:
            stats = analyze_svg(svg_file)
            all_stats[svg_file.stem] = stats
        except Exception as e:
            print(f"\nError analyzing {svg_file.name}: {e}")
    
    # Print individual reports
    for name, stats in all_stats.items():
        print_report(name, stats)
    
    # Summary
    if len(all_stats) > 1:
        print("\n" + "=" * 60)
        print("SUMMARY")
        print("=" * 60)
        
        total_paths = sum(s['paths'] for s in all_stats.values())
        total_circles = sum(s['circles'] for s in all_stats.values())
        total_curves = sum(1 for s in all_stats.values() 
                          if 'C' in s['path_commands'] or 'c' in s['path_commands'])
        total_negative = sum(1 for s in all_stats.values() if s['has_negative_coords'])
        
        print(f"Total files: {len(all_stats)}")
        print(f"Total paths: {total_paths}")
        print(f"Total circles: {total_circles}")
        print(f"Files with curves: {total_curves}")
        print(f"Files with negative coords: {total_negative}")
        
        # All unique commands
        all_commands = set()
        for stats in all_stats.values():
            all_commands.update(stats['path_commands'])
        
        if all_commands:
            print(f"\nAll path commands used: {' '.join(sorted(all_commands))}")
        
        # Files that need special attention
        complex_files = []
        for name, stats in all_stats.items():
            if ('C' in stats['path_commands'] or 'c' in stats['path_commands'] or
                'Q' in stats['path_commands'] or 'q' in stats['path_commands'] or
                'A' in stats['path_commands'] or 'a' in stats['path_commands']):
                complex_files.append(name)
        
        if complex_files:
            print(f"\nComplex files (curves/arcs): {len(complex_files)}")
            for name in complex_files:
                print(f"  • {name}")

if __name__ == '__main__':
    main()
