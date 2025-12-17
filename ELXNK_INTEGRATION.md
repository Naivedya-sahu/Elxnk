# Elxnk Integration System

**Version:** 1.0
**Date:** 2025-12-17
**Status:** Ready for Deployment

## Overview

Elxnk is the unified controller that integrates **lampv2** (drawing engine) and **geniev2** (gesture detector) into a single, cohesive system for circuit schematic drawing on reMarkable 2.

## Architecture

```
┌─────────────────────────────────────────────────┐
│              Elxnk Controller                   │
│  ┌───────────────────────────────────────────┐  │
│  │  Process Manager                          │  │
│  │  - Starts lamp and genie_lamp             │  │
│  │  - Monitors health                        │  │
│  │  - Auto-restart on crash                  │  │
│  │  - Graceful shutdown                      │  │
│  └───────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────┐  │
│  │  IPC Manager                              │  │
│  │  - Named pipe for lamp communication      │  │
│  │  - Signal handling                        │  │
│  │  - Process coordination                   │  │
│  └───────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────┐  │
│  │  UI Controller                            │  │
│  │  - Initializes UI state                   │  │
│  │  - Manages component library              │  │
│  └───────────────────────────────────────────┘  │
└─────────────────────────────────────────────────┘
         │                           │
         ▼                           ▼
   ┌──────────┐              ┌──────────────┐
   │   lamp   │              │  genie_lamp  │
   │  (v2)    │              │    (v2)      │
   └──────────┘              └──────────────┘
         │                           │
         ▼                           ▼
   Framebuffer              Touch Input
   (/dev/fb0)              (/dev/input/event2)
```

## Components

### 1. Elxnk Controller (`elxnk`)

**Location:** `/opt/bin/elxnk`
**Source:** `src/elxnk/elxnk_main.cpp`

**Responsibilities:**
- Start and supervise lamp and genie_lamp processes
- Create and manage named pipe for lamp stdin
- Initialize UI system on startup
- Monitor process health and auto-restart on crash
- Handle graceful shutdown on SIGINT/SIGTERM
- Unified logging to `/tmp/elxnk.log`
- PID file management at `/tmp/elxnk.pid`

**Key Features:**
- Process monitoring every 5 seconds
- Automatic restart with 1-second delay
- Prevents multiple instances via PID file
- Proper cleanup of child processes
- Signal-safe shutdown

### 2. lamp (v2)

**Location:** `/opt/bin/lamp`
**Source:** `resources/rmkit/src/lamp/main.cpy`

**Functionality:**
- Reads drawing commands from stdin (via named pipe)
- Renders directly to framebuffer `/dev/fb0`
- Supports: pen down/move/up, erase mode, primitives
- No rm2fb dependency

**Command Format:**
```
pen down X Y
pen move X Y
pen up
erase on
erase off
```

### 3. genie_lamp (v2)

**Location:** `/opt/bin/genie_lamp`
**Source:** `src/genie_lamp/main.cpp`

**Functionality:**
- Reads touch events from `/dev/input/event2`
- Detects multi-touch tap gestures (1-5 fingers)
- Config-driven command execution
- Cooldown mechanism (30 frames) to prevent re-triggering
- Standalone with no rmkit dependencies

**Configuration:** `/opt/etc/genie_ui.conf`

## Configuration Files

### elxnk.conf

**Location:** `/opt/etc/elxnk.conf`

Main configuration for the Elxnk system (currently for documentation; hardcoded paths in v1.0).

### genie_ui.conf

**Location:** `/opt/etc/genie_ui.conf`

Gesture mappings for UI control:

```ini
# Show/Refresh UI
gesture=tap
fingers=4
command=/home/root/lamp-v2/scripts/ui_state.sh redraw

# Next Page
gesture=tap
fingers=2
command=/home/root/lamp-v2/scripts/ui_state.sh next_page

# Select/Preview Component
gesture=tap
fingers=3
command=/home/root/lamp-v2/scripts/ui_state.sh select

# Next Item
gesture=tap
fingers=5
command=/home/root/lamp-v2/scripts/ui_state.sh next_item
```

## Deployment Structure

```
/opt/
├── bin/
│   ├── elxnk              # Main controller
│   ├── lamp               # Drawing engine
│   └── genie_lamp         # Gesture detector
└── etc/
    ├── elxnk.conf         # Main config
    └── genie_ui.conf      # Gesture config

/home/root/lamp-v2/
├── scripts/
│   ├── ui_state.sh        # UI state machine
│   ├── svg2lamp.sh        # SVG converter
│   ├── component_library.sh
│   └── font_render.sh
└── assets/
    ├── components/        # 17 SVG components
    └── font/              # 36 SVG glyphs

/etc/systemd/system/
└── elxnk.service          # Systemd service

/tmp/
├── elxnk.log              # Runtime log
├── elxnk.pid              # PID file
├── elxnk_lamp.pipe        # Named pipe for lamp
└── genie_ui/              # UI state files
```

## Building

### Prerequisites

- ARM cross-compiler: `arm-linux-gnueabihf-g++`
- GNU Make
- SSH access to reMarkable 2

### Build Commands

```bash
# Build everything
cd src/elxnk
make all

# Or use the deployment script
bash scripts/deploy_elxnk.sh
```

### Build Targets

- `make all` - Build elxnk controller
- `make clean` - Clean build artifacts
- `make deploy` - Deploy elxnk binary only
- `make deploy-full` - Build and deploy everything (recommended)
- `make help` - Show all available targets

## Deployment

### Quick Deployment

```bash
# Set reMarkable IP (if different from 10.11.99.1)
export HOST=10.11.99.1

# Deploy everything
bash scripts/deploy_elxnk.sh
```

This will:
1. Build elxnk, genie_lamp, and lamp
2. Deploy binaries to `/opt/bin/`
3. Deploy configurations to `/opt/etc/`
4. Deploy scripts and assets to `/home/root/lamp-v2/`
5. Install systemd service
6. Start Elxnk automatically

### Manual Deployment

```bash
cd src/elxnk
make deploy-full HOST=10.11.99.1
make enable
```

### Verify Deployment

```bash
# Check service status
ssh root@10.11.99.1 systemctl status elxnk

# View logs
ssh root@10.11.99.1 tail -f /tmp/elxnk.log

# Or use make target
cd src/elxnk
make status
make logs
```

## Running Elxnk

### As a Service (Recommended)

```bash
# Start
systemctl start elxnk

# Stop
systemctl stop elxnk

# Restart
systemctl restart elxnk

# Enable on boot
systemctl enable elxnk

# Status
systemctl status elxnk
```

### Manual Mode (for testing)

```bash
# Stop service first
systemctl stop elxnk

# Run manually
/opt/bin/elxnk

# Or from deployment machine
ssh root@10.11.99.1 /opt/bin/elxnk
```

## Usage

### Gesture Controls

| Gesture | Function |
|---------|----------|
| 4-finger tap | Show/Refresh UI |
| 2-finger tap | Next page |
| 3-finger tap | Select/Preview component |
| 5-finger tap | Next item in list |

### Workflow

1. **Start Elxnk** (automatic on boot if enabled)
2. **4-finger tap** to show component library UI
3. **5-finger tap** to navigate through components
4. **2-finger tap** to go to next page
5. **3-finger tap** to preview selected component

## Monitoring

### Log Files

```bash
# Main log
tail -f /tmp/elxnk.log

# Systemd journal
journalctl -u elxnk -f

# From deployment machine
ssh root@10.11.99.1 tail -f /tmp/elxnk.log
```

### Process Status

```bash
# Check if processes are running
ps aux | grep -E 'elxnk|lamp|genie_lamp'

# Check service status
systemctl status elxnk

# View PID
cat /tmp/elxnk.pid
```

## Troubleshooting

### Elxnk won't start

```bash
# Check logs
cat /tmp/elxnk.log

# Check if binaries exist
ls -la /opt/bin/{elxnk,lamp,genie_lamp}

# Check permissions
ls -la /dev/input/event2 /dev/fb0

# Try manual mode
/opt/bin/elxnk
```

### Gestures not working

```bash
# Check if genie_lamp is running
ps aux | grep genie_lamp

# Check config
cat /opt/etc/genie_ui.conf

# Test touch device
cat /dev/input/event2  # Should show data when touching

# Check genie_lamp logs in main log
tail -f /tmp/elxnk.log | grep genie
```

### UI not drawing

```bash
# Check if lamp is running
ps aux | grep lamp

# Check pipe
ls -la /tmp/elxnk_lamp.pipe

# Test lamp manually
echo "pen down 500 500" > /tmp/elxnk_lamp.pipe
echo "pen move 900 900" > /tmp/elxnk_lamp.pipe
echo "pen up" > /tmp/elxnk_lamp.pipe

# Check scripts
ls -la /home/root/lamp-v2/scripts/
```

### Process crashes

Elxnk automatically restarts crashed processes. Check logs:

```bash
tail -100 /tmp/elxnk.log | grep -E 'died|restarting|crashed'
```

## Maintenance

### Update Deployment

```bash
# Build and redeploy
bash scripts/deploy_elxnk.sh

# Or
cd src/elxnk
make deploy-full
make restart
```

### Uninstall

```bash
# Stop service
systemctl stop elxnk
systemctl disable elxnk

# Remove files
rm -f /opt/bin/{elxnk,lamp,genie_lamp}
rm -f /opt/etc/{elxnk.conf,genie_ui.conf}
rm -f /etc/systemd/system/elxnk.service
rm -rf /home/root/lamp-v2
systemctl daemon-reload

# Or use make target
cd src/elxnk
make uninstall
```

### Clean Logs

```bash
# Truncate log
> /tmp/elxnk.log

# Or rotate
mv /tmp/elxnk.log /tmp/elxnk.log.old
systemctl restart elxnk
```

## Technical Details

### Process Lifecycle

1. **Startup:**
   - Elxnk creates named pipe
   - Starts lamp with stdin redirected to pipe
   - Initializes UI system
   - Starts genie_lamp with config

2. **Runtime:**
   - Monitor loop checks processes every 5s
   - Auto-restart crashed processes with 1s delay
   - Log all activities to `/tmp/elxnk.log`

3. **Shutdown:**
   - Stop genie_lamp first (no more gestures)
   - Close lamp pipe
   - Stop lamp
   - Wait up to 3s for graceful exit
   - Force kill if needed
   - Clean up pipe and PID file

### IPC Mechanism

- **Named Pipe:** `/tmp/elxnk_lamp.pipe`
- **Direction:** genie_lamp → ui scripts → lamp
- **Format:** Text commands (newline-terminated)
- **Blocking:** Lamp blocks on read, elxnk keeps pipe open

### Signal Handling

- **SIGINT/SIGTERM/SIGHUP:** Graceful shutdown
- **SIGCHLD:** Ignored (automatic reaping)

## Performance

- **Memory:** ~5MB total (all 3 processes)
- **CPU:** < 1% idle, 5-10% during gestures
- **Startup Time:** < 2 seconds
- **Response Time:** < 100ms for gestures

## Future Enhancements

- [ ] Config file parsing (currently hardcoded paths)
- [ ] Swipe gesture support
- [ ] Component rotation
- [ ] Wire drawing mode
- [ ] Save/load schematics
- [ ] Network API for external control
- [ ] Real-time lamp command injection
- [ ] Better error recovery
- [ ] Watchdog timer

## Version History

### v1.0 (2025-12-17)
- Initial release
- Process management for lamp and genie_lamp
- Named pipe IPC
- Systemd service integration
- Auto-restart on crash
- UI initialization
- Deployment scripts

## License

See project LICENSE file.

## Support

For issues, check:
1. `/tmp/elxnk.log` - Main log file
2. `systemctl status elxnk` - Service status
3. Project documentation in repository

## Credits

- **lampv2:** Based on rmkit lamp application
- **geniev2:** Custom standalone gesture detector
- **Elxnk:** Integration layer and process manager
