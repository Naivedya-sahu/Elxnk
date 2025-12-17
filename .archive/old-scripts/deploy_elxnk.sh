#!/bin/bash
# Elxnk Deployment Script
# Builds and deploys the complete Elxnk system to reMarkable 2

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
HOST="${HOST:-10.11.99.1}"

# Deployment paths
INSTALL_BIN="/opt/bin"
INSTALL_ETC="/opt/etc"
INSTALL_SYSTEMD="/etc/systemd/system"
DEPLOY_DIR="/home/root/lamp-v2"

# Print colored message
log() {
    local color=$1
    shift
    echo -e "${color}$@${NC}"
}

info() { log "$BLUE" "[INFO] $@"; }
success() { log "$GREEN" "[SUCCESS] $@"; }
warn() { log "$YELLOW" "[WARN] $@"; }
error() { log "$RED" "[ERROR] $@"; }

# Check if reMarkable is reachable
check_remarkable() {
    info "Checking connection to reMarkable 2 at $HOST..."
    if ! ping -c 1 -W 2 "$HOST" &> /dev/null; then
        error "Cannot reach reMarkable at $HOST"
        exit 1
    fi

    if ! ssh -o ConnectTimeout=5 root@"$HOST" "echo connected" &> /dev/null; then
        error "Cannot SSH to reMarkable at $HOST"
        exit 1
    fi

    success "Connected to reMarkable 2"
}

# Build all components
build_all() {
    info "Building Elxnk components..."

    # Build elxnk main controller
    info "  - Building elxnk controller..."
    cd "$PROJECT_ROOT/src/elxnk"
    make clean
    make all

    # Build genie_lamp
    info "  - Building genie_lamp..."
    cd "$PROJECT_ROOT/src/genie_lamp"
    make clean
    make all

    # Build lamp (from rmkit)
    info "  - Building lamp from rmkit..."
    cd "$PROJECT_ROOT/resources/rmkit"
    make lamp TARGET=rm

    success "All components built successfully"
}

# Deploy binaries
deploy_binaries() {
    info "Deploying binaries to reMarkable..."

    # Create directories
    ssh root@"$HOST" "mkdir -p $INSTALL_BIN $INSTALL_ETC $DEPLOY_DIR"

    # Deploy elxnk
    info "  - Deploying elxnk..."
    scp "$PROJECT_ROOT/build/bin/elxnk" root@"$HOST":"$INSTALL_BIN/"

    # Deploy genie_lamp
    info "  - Deploying genie_lamp..."
    scp "$PROJECT_ROOT/src/genie_lamp/genie_lamp" root@"$HOST":"$INSTALL_BIN/"

    # Deploy lamp
    info "  - Deploying lamp..."
    scp "$PROJECT_ROOT/resources/rmkit/src/build/lamp" root@"$HOST":"$INSTALL_BIN/"

    # Set permissions
    ssh root@"$HOST" "chmod +x $INSTALL_BIN/elxnk $INSTALL_BIN/genie_lamp $INSTALL_BIN/lamp"

    success "Binaries deployed"
}

# Deploy configurations
deploy_configs() {
    info "Deploying configuration files..."

    # Deploy elxnk config
    info "  - Deploying elxnk.conf..."
    scp "$PROJECT_ROOT/src/config/elxnk.conf" root@"$HOST":"$INSTALL_ETC/"

    # Deploy genie UI config
    info "  - Deploying genie_ui.conf..."
    scp "$PROJECT_ROOT/src/genie_lamp/ui.conf" root@"$HOST":"$INSTALL_ETC/genie_ui.conf"

    success "Configurations deployed"
}

# Deploy scripts and assets
deploy_assets() {
    info "Deploying scripts and assets..."

    # Create directories
    ssh root@"$HOST" "mkdir -p $DEPLOY_DIR/scripts $DEPLOY_DIR/assets/components $DEPLOY_DIR/assets/font"

    # Deploy scripts
    info "  - Deploying utility scripts..."
    scp "$PROJECT_ROOT/scripts/ui_state.sh" root@"$HOST":"$DEPLOY_DIR/scripts/"
    scp "$PROJECT_ROOT/scripts/svg2lamp.sh" root@"$HOST":"$DEPLOY_DIR/scripts/"
    scp "$PROJECT_ROOT/scripts/component_library.sh" root@"$HOST":"$DEPLOY_DIR/scripts/"
    scp "$PROJECT_ROOT/scripts/font_render.sh" root@"$HOST":"$DEPLOY_DIR/scripts/"

    # Set script permissions
    ssh root@"$HOST" "chmod +x $DEPLOY_DIR/scripts/*.sh"

    # Deploy component assets
    info "  - Deploying component library..."
    scp "$PROJECT_ROOT/assets/components/"*.svg root@"$HOST":"$DEPLOY_DIR/assets/components/" 2>/dev/null || warn "No components found"

    # Deploy font assets
    info "  - Deploying font glyphs..."
    scp "$PROJECT_ROOT/assets/font/"*.svg root@"$HOST":"$DEPLOY_DIR/assets/font/" 2>/dev/null || warn "No fonts found"

    success "Assets deployed"
}

# Install systemd service
install_service() {
    info "Installing systemd service..."

    # Stop existing service if running
    ssh root@"$HOST" "systemctl stop elxnk 2>/dev/null || true"

    # Deploy service file
    scp "$PROJECT_ROOT/src/config/elxnk.service" root@"$HOST":"$INSTALL_SYSTEMD/"

    # Reload systemd
    ssh root@"$HOST" "systemctl daemon-reload"

    success "Systemd service installed"
}

# Enable and start service
start_service() {
    info "Enabling and starting Elxnk service..."

    ssh root@"$HOST" "systemctl enable elxnk"
    ssh root@"$HOST" "systemctl start elxnk"

    sleep 2

    # Check status
    if ssh root@"$HOST" "systemctl is-active elxnk" &> /dev/null; then
        success "Elxnk service is running"
    else
        error "Elxnk service failed to start"
        ssh root@"$HOST" "systemctl status elxnk"
        exit 1
    fi
}

# Show status
show_status() {
    info "Elxnk Status:"
    echo ""
    ssh root@"$HOST" "systemctl status elxnk"
    echo ""
    info "Recent logs:"
    ssh root@"$HOST" "tail -20 /tmp/elxnk.log"
}

# Main deployment flow
main() {
    echo "================================================"
    echo "  Elxnk Deployment Script"
    echo "  Target: $HOST"
    echo "================================================"
    echo ""

    # Check connection
    check_remarkable

    # Build
    build_all

    # Deploy
    deploy_binaries
    deploy_configs
    deploy_assets

    # Install service
    install_service

    # Start (if requested)
    if [ "${START_SERVICE:-yes}" = "yes" ]; then
        start_service
        show_status
    else
        info "Service installed but not started (START_SERVICE=no)"
        info "To start manually: ssh root@$HOST systemctl start elxnk"
    fi

    echo ""
    echo "================================================"
    success "Deployment Complete!"
    echo "================================================"
    echo ""
    echo "Next steps:"
    echo "  - View logs: ssh root@$HOST tail -f /tmp/elxnk.log"
    echo "  - Service status: ssh root@$HOST systemctl status elxnk"
    echo "  - Restart: ssh root@$HOST systemctl restart elxnk"
    echo "  - Stop: ssh root@$HOST systemctl stop elxnk"
    echo ""
    echo "Gestures:"
    echo "  - 4-finger tap: Show/Refresh UI"
    echo "  - 2-finger tap: Next page"
    echo "  - 3-finger tap: Select/Preview component"
    echo "  - 5-finger tap: Next item"
    echo ""
}

# Run main
main "$@"
