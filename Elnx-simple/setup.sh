#!/bin/bash
# Elnx-Simple Setup Script
# Prepares the environment and verifies everything works

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "========================================="
echo "  Elnx-Simple Setup & Verification"
echo "========================================="
echo ""

# Check Python 3
echo "[1/5] Checking Python 3..."
if ! command -v python3 &> /dev/null; then
    echo "❌ Error: Python 3 is not installed"
    echo "   Install with: apt-get install python3 (Debian/Ubuntu)"
    echo "              or: brew install python3 (macOS)"
    exit 1
fi
PYTHON_VERSION=$(python3 --version)
echo "✓ Found: $PYTHON_VERSION"
echo ""

# Check SSH
echo "[2/5] Checking SSH client..."
if ! command -v ssh &> /dev/null; then
    echo "⚠️  Warning: SSH client is not installed"
    echo "   You'll need SSH to connect to reMarkable 2"
    echo "   Install with: apt-get install openssh-client (Debian/Ubuntu)"
    echo "              or: brew install openssh (macOS)"
    echo "   Continuing setup..."
else
    echo "✓ SSH client available"
fi
echo ""

# Verify directory structure
echo "[3/5] Verifying directory structure..."
REQUIRED_DIRS=("bin" "lib" "scripts" "lamp")
MISSING_DIRS=()

for dir in "${REQUIRED_DIRS[@]}"; do
    if [ ! -d "$dir" ]; then
        MISSING_DIRS+=("$dir")
    fi
done

if [ ${#MISSING_DIRS[@]} -gt 0 ]; then
    echo "❌ Error: Missing directories: ${MISSING_DIRS[*]}"
    exit 1
fi
echo "✓ All directories present"
echo ""

# Verify files
echo "[4/5] Verifying required files..."
REQUIRED_FILES=(
    "bin/render_json.py"
    "bin/draw_remote"
    "lib/components.json"
    "lib/fonts.json"
    "scripts/draw_component.sh"
    "scripts/draw_text.sh"
    "lamp/main.cpp"
    "lamp/Makefile"
)

MISSING_FILES=()
for file in "${REQUIRED_FILES[@]}"; do
    if [ ! -f "$file" ]; then
        MISSING_FILES+=("$file")
    fi
done

if [ ${#MISSING_FILES[@]} -gt 0 ]; then
    echo "❌ Error: Missing files:"
    for file in "${MISSING_FILES[@]}"; do
        echo "   - $file"
    done
    exit 1
fi
echo "✓ All required files present"
echo ""

# Test JSON rendering
echo "[5/5] Testing JSON rendering..."
if ! python3 bin/render_json.py list > /dev/null 2>&1; then
    echo "❌ Error: render_json.py failed to run"
    echo "   Try running manually: python3 bin/render_json.py list"
    exit 1
fi

# Test component rendering
if ! python3 bin/render_json.py component R 500 400 1.0 > /dev/null 2>&1; then
    echo "❌ Error: Failed to render component"
    exit 1
fi

# Test text rendering
if ! python3 bin/render_json.py text "AB" 300 200 1.0 > /dev/null 2>&1; then
    echo "❌ Error: Failed to render text"
    exit 1
fi

echo "✓ JSON rendering works correctly"
echo ""

# Make scripts executable
chmod +x bin/* scripts/* 2>/dev/null || true

echo "========================================="
echo "  ✅ Setup Complete!"
echo "========================================="
echo ""
echo "Next steps:"
echo ""
echo "1. Set your reMarkable 2 IP address:"
echo "   export RM2_HOST=10.11.99.1"
echo ""
echo "2. Build and deploy lamp:"
echo "   cd lamp"
echo "   make              # Build for ARM"
echo "   make deploy       # Deploy to reMarkable 2"
echo ""
echo "3. Test drawing:"
echo "   cd .."
echo "   ./scripts/draw_component.sh R 500 400 1.0"
echo ""
echo "For detailed instructions, see README.md"
echo ""
