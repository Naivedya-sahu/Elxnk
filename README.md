# Elxnk - Circuit Schematic Drawing for reMarkable 2

Integrated system combining **lampv2** (drawing engine), **geniev2** (gesture detector), and unified process control for drawing circuit schematics on reMarkable 2 e-ink tablets.

## Quick Start

```bash
# Build and deploy
cd src
make deploy

# Check status
make status

# View logs
make logs
```

## Features

- **17 Circuit Components**: R, C, L, Diodes, Transistors, Op-amps, etc.
- **36 Font Glyphs**: A-Z, 0-9 for text labels
- **Multi-touch Gestures**: 2-5 finger tap controls
- **Auto-restart**: Crashed processes restart automatically
- **Embedded Library**: No runtime SVG processing needed

## Architecture

```
Elxnk Controller (Process Manager)
├── lamp → Framebuffer (/dev/fb0)
├── genie_lamp → Touch Input (/dev/input/event2)
└── Embedded Component Library (C header)
```

## Build System

### Prerequisites

- `arm-linux-gnueabihf-g++` (ARM cross-compiler)
- `python3` (for build-time SVG conversion)
- `make`
- SSH access to reMarkable 2

### Build Commands

```bash
cd src

# Build everything
make all

# Generate library from SVGs
make library

# Build individual components
make elxnk    # Controller
make genie    # Gesture detector
make lamp     # Drawing engine

# Clean
make clean
```

### Deployment

```bash
# Full deployment (builds + installs + starts service)
make deploy

# Just install binaries
make install

# Service control
make start | stop | restart | status

# Uninstall
make uninstall
```

## Gesture Controls

| Gesture | Function |
|---------|----------|
| 4-finger tap | Show/Refresh UI |
| 2-finger tap | Next page |
| 3-finger tap | Select/Preview component |
| 5-finger tap | Next item |

## Components

**Passive:** R, C, L, NP_C, P_C, R_TRIM
**Semiconductors:** D, ZD, NPN, PNP, NMOS, PMOS
**Sources:** VDC, VAC, VAR
**Other:** GND, TX, OPAMP

## File Structure

```
Elxnk/                    # Root (6 items - minimized!)
├── README.md             # Documentation
├── CLEANUP_NOTES.md      # Refactoring notes
├── REFACTOR_SUMMARY.md   # Summary of changes
├── assets/               # SVG sources (build-time only)
│   ├── components/       # 17 circuit components
│   └── font/             # 36 font glyphs
├── tools/
│   └── svg2header.py     # SVG → C header converter (build-time)
└── src/                  # All source code
    ├── Makefile          # Unified build system
    ├── lamp/
    │   └── main.cpp      # Standalone drawing engine (361 lines, pure C++)
    ├── genie_lamp/
    │   ├── main.cpp      # Gesture detector (254 lines)
    │   └── ui.conf       # Gesture mappings
    ├── elxnk/
    │   ├── elxnk_main.cpp    # Main controller (480 lines)
    │   └── component_library.h  # Auto-generated (27KB, 888 commands)
    └── config/
        ├── elxnk.conf    # System config
        └── elxnk.service # Systemd service
```

## Deployment Structure (reMarkable 2)

```
/opt/bin/               # Binaries
├── elxnk              # Main controller
├── lamp               # Drawing engine
└── genie_lamp         # Gesture detector

/opt/etc/              # Configuration
├── elxnk.conf
└── genie_ui.conf
```

## How It Works

### Build Time
1. `svg2header.py` converts all SVG components/fonts to C arrays
2. `component_library.h` contains embedded lamp commands
3. Standalone `lamp` built from pure C++ (no rmkit/external deps)
4. All three binaries compiled with embedded data

### Runtime
1. `elxnk` starts and manages `lamp` + `genie_lamp`
2. `genie_lamp` detects gestures from touch input
3. Gestures trigger commands that send lamp drawing commands
4. `lamp` renders to framebuffer using embedded component data
5. `elxnk` monitors health and auto-restarts on crash

### No Runtime Dependencies
✅ No SVG parsing | ✅ No bash scripts | ✅ No Python | ✅ No rmkit | ✅ All data embedded

## Monitoring

```bash
# Service status
make status

# Live logs
make logs

# Or directly on device
ssh root@10.11.99.1 tail -f /tmp/elxnk.log
```

## Troubleshooting

**Service won't start:** `make status` then check `/tmp/elxnk.log`
**Gestures not working:** Check `ps aux | grep genie_lamp` and `/dev/input/event2`
**Drawing not working:** Check `ps aux | grep lamp` and `/tmp/elxnk_lamp.pipe`

## Development

**Add Components:** Place SVG in `assets/components/` → `make library deploy`
**Add Fonts:** Place SVG in `assets/font/NAME.svg` → `make library deploy`
**Modify Gestures:** Edit `src/genie_lamp/ui.conf` → `make deploy`

## Performance

**Memory:** ~5MB | **CPU:** <1% idle | **Startup:** <2s | **Library:** 27KB (888 cmds)

## License

See LICENSE file.
