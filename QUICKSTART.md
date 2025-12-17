# Elxnk Quick Start Guide

## What is Elxnk?

Elxnk is a unified controller that integrates **lampv2** (drawing engine) and **geniev2** (gesture detector) to create a complete circuit schematic drawing system for reMarkable 2.

## Quick Deploy

```bash
# 1. Set your reMarkable IP (if different)
export HOST=10.11.99.1

# 2. Run deployment script
bash scripts/deploy_elxnk.sh
```

That's it! Elxnk will be built, deployed, and started automatically.

## Quick Commands

```bash
# View logs
ssh root@10.11.99.1 tail -f /tmp/elxnk.log

# Check status
ssh root@10.11.99.1 systemctl status elxnk

# Restart
ssh root@10.11.99.1 systemctl restart elxnk

# Stop
ssh root@10.11.99.1 systemctl stop elxnk
```

## Gestures on reMarkable

| Fingers | Action |
|---------|--------|
| 4-tap | Show UI |
| 5-tap | Next item |
| 2-tap | Next page |
| 3-tap | Preview component |

## What Gets Deployed?

```
/opt/bin/
  ├── elxnk          # Main controller
  ├── lamp           # Drawing engine
  └── genie_lamp     # Gesture detector

/opt/etc/
  ├── elxnk.conf     # Main config
  └── genie_ui.conf  # Gesture config

/home/root/lamp-v2/
  ├── scripts/       # UI and rendering scripts
  └── assets/        # Components and fonts
```

## Components Included

- **Passive:** R, C, L, NP_C, P_C, R_TRIM
- **Semiconductors:** D, ZD, NPN, PNP, NMOS, PMOS
- **Sources:** VDC, VAC, VAR
- **Other:** GND, TX, OPAMP

## System Architecture

```
Elxnk Controller
    ├── Manages lamp (drawing)
    ├── Manages genie_lamp (gestures)
    ├── Monitors health & auto-restart
    └── Logs to /tmp/elxnk.log

Touch Input → genie_lamp → UI Scripts → lamp → Framebuffer
```

## Build Manually

```bash
# Build elxnk
cd src/elxnk
make all

# Build genie_lamp
cd src/genie_lamp
make all

# Build lamp
cd resources/rmkit
make lamp TARGET=rm
```

## Deploy Manually

```bash
cd src/elxnk
make deploy-full HOST=10.11.99.1
make enable
```

## Troubleshooting

### Not drawing?
```bash
# Check if lamp is running
ssh root@10.11.99.1 "ps aux | grep lamp"

# Check logs
ssh root@10.11.99.1 tail -50 /tmp/elxnk.log
```

### Gestures not working?
```bash
# Check if genie_lamp is running
ssh root@10.11.99.1 "ps aux | grep genie"

# Check touch device
ssh root@10.11.99.1 "ls -la /dev/input/event2"
```

### Service won't start?
```bash
# Check service status
ssh root@10.11.99.1 systemctl status elxnk

# Try manual mode
ssh root@10.11.99.1 /opt/bin/elxnk
```

## Uninstall

```bash
cd src/elxnk
make uninstall
```

## More Info

See `ELXNK_INTEGRATION.md` for complete documentation.

## Current Status

✅ lampv2 - Deployed and functional
✅ geniev2 - Deployed and functional
✅ Elxnk integration - Ready for deployment
✅ UI system - Ready
✅ Component library - 17 components
✅ Deployment automation - Complete

## Next Steps

1. Deploy: `bash scripts/deploy_elxnk.sh`
2. Test gestures on reMarkable
3. Try drawing components
4. Check logs if issues arise

Happy drawing! 🎨
