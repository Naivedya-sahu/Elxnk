# Elnx-Simple Quick Start Guide

Get started with Elnx-Simple in 5 minutes!

## Prerequisites

- **Linux or macOS** computer
- **Python 3** (usually pre-installed)
- **SSH client** (usually pre-installed)
- **reMarkable 2** tablet with WiFi connection
- **SSH access** to reMarkable 2 (passwordless recommended)

## Step 1: Download

```bash
# Clone or download the Elnx-Simple directory
git clone --single-branch --branch claude/elnx-simple-gdoVV \
  https://github.com/Naivedya-sahu/Elxnk.git

cd Elxnk/Elnx-simple
```

Or download ZIP and extract:
```bash
unzip Elnx-simple.zip
cd Elnx-simple
```

## Step 2: Verify Setup

```bash
# Run setup verification
./setup.sh
```

Expected output:
```
=========================================
  Elnx-Simple Setup & Verification
=========================================

[1/5] Checking Python 3...
✓ Found: Python 3.x.x

[2/5] Checking SSH client...
✓ SSH client available

[3/5] Verifying directory structure...
✓ All directories present

[4/5] Verifying required files...
✓ All required files present

[5/5] Testing JSON rendering...
✓ JSON rendering works correctly

=========================================
  ✅ Setup Complete!
=========================================
```

## Step 3: Test Locally (No reMarkable Needed)

```bash
# Test rendering without reMarkable 2
./test_local.sh
```

This verifies all components render correctly before deploying.

## Step 4: Configure reMarkable IP

Find your reMarkable 2 IP address:
- **USB**: Usually `10.11.99.1`
- **WiFi**: Settings → Help → Copyrights → General information

```bash
# Set IP address
export RM2_HOST=10.11.99.1
# Or for WiFi:
export RM2_HOST=192.168.1.xxx
```

Add to `~/.bashrc` or `~/.zshrc` to make permanent:
```bash
echo 'export RM2_HOST=10.11.99.1' >> ~/.bashrc
source ~/.bashrc
```

## Step 5: Build and Deploy Lamp

```bash
# Navigate to lamp directory
cd lamp

# Build lamp for reMarkable 2
make

# Deploy to device
make deploy

# Test it works
make test
```

Expected output:
```
✓ Lamp deployed to 10.11.99.1:/opt/bin/lamp
Testing lamp on reMarkable 2...
✓ Test complete - check screen for diagonal line
```

## Step 6: Draw Your First Component

```bash
# Go back to Elnx-simple directory
cd ..

# Draw a resistor at center of screen
./scripts/draw_component.sh R 700 900 1.5
```

You should see a resistor drawn on your reMarkable 2 screen!

## Step 7: Draw Text

```bash
./scripts/draw_text.sh "Hello RM2" 500 500 1.0
```

## Step 8: Draw a Circuit

```bash
./scripts/draw_circuit.sh
```

This draws a complete RC circuit with labels.

## Usage Examples

### List Available Components

```bash
./bin/render_json.py list
```

### Draw Specific Components

```bash
# Syntax: draw_component.sh <name> <x> <y> [scale]

# Resistor
./scripts/draw_component.sh R 500 400 1.0

# Capacitor (larger)
./scripts/draw_component.sh C 600 400 2.0

# Diode
./scripts/draw_component.sh D 700 400 1.5

# Ground
./scripts/draw_component.sh GND 800 400 1.0

# MOSFET
./scripts/draw_component.sh NMOS 900 400 1.0
```

### Draw Text at Different Scales

```bash
# Normal size
./scripts/draw_text.sh "R1" 500 300 1.0

# Larger
./scripts/draw_text.sh "100K" 500 350 1.5

# Smaller
./scripts/draw_text.sh "5V" 500 380 0.7
```

### Clear Screen Area

```bash
# Clear rectangle: x, y, width, height
./scripts/clear_area.sh 100 100 800 600
```

## Screen Coordinates

reMarkable 2 screen: **1404 × 1872 pixels**

```
(0,0)                    (1404,0)
  ┌─────────────────────────┐
  │                         │
  │                         │
  │      (702,936)          │ 1872px
  │       CENTER            │
  │                         │
  │                         │
  └─────────────────────────┘
(0,1872)               (1404,1872)
       1404px
```

**Safe drawing area**: `200 < x < 1200`, `200 < y < 1670`

## Creating Custom Circuits

Create a new bash script:

```bash
#!/bin/bash
# my_circuit.sh

set -euo pipefail
SCRIPT_DIR="$(dirname "$0")"

# Use center of screen
CX=702
CY=936

# Draw voltage divider
$SCRIPT_DIR/draw_component.sh R $CX $((CY - 200)) 1.0  # R1
$SCRIPT_DIR/draw_component.sh R $CX $CY 1.0            # R2
$SCRIPT_DIR/draw_component.sh GND $CX $((CY + 200)) 1.0

# Labels
$SCRIPT_DIR/draw_text.sh "R1" $((CX + 50)) $((CY - 200)) 0.8
$SCRIPT_DIR/draw_text.sh "10K" $((CX + 50)) $((CY - 170)) 0.7
$SCRIPT_DIR/draw_text.sh "R2" $((CX + 50)) $CY 0.8
$SCRIPT_DIR/draw_text.sh "10K" $((CX + 50)) $((CY + 30)) 0.7
```

Make executable and run:
```bash
chmod +x my_circuit.sh
./my_circuit.sh
```

## Troubleshooting

### Cannot connect to reMarkable

```bash
# Check IP
ping 10.11.99.1

# Test SSH
ssh root@10.11.99.1

# If using WiFi, find correct IP in Settings
```

### Lamp not found on device

```bash
# Redeploy lamp
cd lamp
make deploy
```

### Component doesn't render

```bash
# Test command generation locally
./bin/render_json.py component R 500 400 1.0

# If this works, problem is with SSH connection
# Check RM2_HOST environment variable
```

### Python errors

```bash
# Check Python 3 is installed
python3 --version

# Test JSON library
python3 -c "import json; print('OK')"
```

## Advanced Usage

### Generate Commands Without Sending

```bash
# Just generate lamp commands (don't send to device)
./bin/render_json.py component R 500 400 1.0 > my_commands.txt
cat my_commands.txt

# Send later
cat my_commands.txt | ssh root@10.11.99.1 /opt/bin/lamp
```

### Use Different Host

```bash
# Temporary
./scripts/draw_component.sh R 500 400 1.0
# Then in draw_remote, use --host option

# Or set for this session
export RM2_HOST=192.168.1.100
./scripts/draw_component.sh R 500 400 1.0
```

### Batch Drawing

```bash
# Create a file with multiple draw commands
cat > batch.sh << 'EOF'
#!/bin/bash
for i in {0..9}; do
  ./scripts/draw_component.sh R $((200 + i*100)) 500 1.0
  sleep 0.5
done
EOF

chmod +x batch.sh
./batch.sh
```

## Next Steps

1. **Explore components**: Run `./bin/render_json.py list`
2. **Edit JSON library**: Add your own components in `lib/components.json`
3. **Create circuits**: Build complex schematics from simple commands
4. **Read full docs**: See `README.md` for detailed information

## Common Workflows

### Quick Test on reMarkable

```bash
export RM2_HOST=10.11.99.1
./scripts/draw_component.sh R 700 900 1.5
```

### Add New Component to Library

1. Edit `lib/components.json`
2. Add component definition
3. Test: `./bin/render_json.py component NEWCOMP 500 500 1.0`
4. Draw: `./scripts/draw_component.sh NEWCOMP 500 500 1.0`

No rebuild needed!

### Create Circuit Template

1. Copy `scripts/draw_circuit.sh` to `my_circuit.sh`
2. Modify component positions
3. Run `./my_circuit.sh`

## Getting Help

- **Setup issues**: Run `./setup.sh` and check output
- **Local testing**: Run `./test_local.sh` to verify without reMarkable
- **Full documentation**: See `README.md`
- **Development info**: See `../ELNX_SIMPLE_DEVELOPMENT.md`

## Summary

```bash
# One-time setup
./setup.sh
cd lamp && make deploy && cd ..

# Draw anything
export RM2_HOST=10.11.99.1
./scripts/draw_component.sh R 700 900 1.5
./scripts/draw_text.sh "Hello" 500 500 1.0
./scripts/draw_circuit.sh

# That's it!
```

Happy drawing! 🎨📐
