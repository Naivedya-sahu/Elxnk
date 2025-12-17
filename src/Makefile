# Unified Elxnk Build System
# Builds lamp, genie_lamp, and elxnk from single Makefile

# Cross-compiler for ARM (reMarkable 2)
CXX = arm-linux-gnueabihf-g++
CXXFLAGS = -O2 -std=c++11 -Wall -Wextra
PYTHON = python3

# Directories
BUILD_DIR = ../build
BIN_DIR = $(BUILD_DIR)/bin
TOOLS_DIR = ../tools
ASSETS_DIR = ../assets
# Removed: RMKIT_DIR = ../resources/rmkit

# Target binaries
ELXNK_BIN = $(BIN_DIR)/elxnk
GENIE_BIN = $(BIN_DIR)/genie_lamp
LAMP_BIN = $(BIN_DIR)/lamp

# Source files
ELXNK_SRC = elxnk/elxnk_main.cpp
GENIE_SRC = genie_lamp/main.cpp
LAMP_SRC = lamp/main.cpp
ELXNK_LIB = elxnk/component_library.h

# Deployment config
HOST ?= 10.11.99.1
INSTALL_BIN = /opt/bin
INSTALL_ETC = /opt/etc
INSTALL_SYSTEMD = /etc/systemd/system
DEPLOY_DIR = /home/root/lamp-v2

# Build targets
.PHONY: all clean library elxnk genie lamp install deploy status help

all: library elxnk genie lamp
	@echo ""
	@echo "=== Build Complete ==="
	@echo "Binaries:"
	@ls -lh $(BIN_DIR)
	@echo ""

# Generate component library header from SVG assets
library: $(ELXNK_LIB)

$(ELXNK_LIB): $(TOOLS_DIR)/svg2header.py
	@echo "Generating component library from SVG assets..."
	@mkdir -p $(dir $@)
	$(PYTHON) $(TOOLS_DIR)/svg2header.py \
		$(ASSETS_DIR)/components \
		$(ASSETS_DIR)/font \
		$@
	@echo "Library generated: $@"

# Build elxnk controller
elxnk: $(ELXNK_BIN)

$(ELXNK_BIN): $(ELXNK_SRC) $(ELXNK_LIB) | $(BIN_DIR)
	@echo "Building elxnk..."
	$(CXX) $(CXXFLAGS) -o $@ $(ELXNK_SRC)
	@echo "Built: $@"

# Build genie_lamp gesture detector
genie: $(GENIE_BIN)

$(GENIE_BIN): $(GENIE_SRC) | $(BIN_DIR)
	@echo "Building genie_lamp..."
	$(CXX) $(CXXFLAGS) -o $@ $(GENIE_SRC)
	@echo "Built: $@"

# Build lamp drawing engine (standalone)
lamp: $(LAMP_BIN)

$(LAMP_BIN): lamp/main.cpp | $(BIN_DIR)
	@echo "Building lamp (standalone)..."
	$(CXX) $(CXXFLAGS) -o $@ lamp/main.cpp
	@echo "Built: $@"

# Create build directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean all build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)
	rm -f $(ELXNK_LIB)
	@echo "Clean complete"

# Install - deploy binaries only
install: all
	@echo "Installing binaries to reMarkable 2..."
	ssh root@$(HOST) "mkdir -p $(INSTALL_BIN)"
	scp $(ELXNK_BIN) root@$(HOST):$(INSTALL_BIN)/elxnk
	scp $(GENIE_BIN) root@$(HOST):$(INSTALL_BIN)/genie_lamp
	scp $(LAMP_BIN) root@$(HOST):$(INSTALL_BIN)/lamp
	scp ui_state.sh root@$(HOST):$(INSTALL_BIN)/ui_state.sh
	ssh root@$(HOST) "chmod +x $(INSTALL_BIN)/{elxnk,genie_lamp,lamp,ui_state.sh}"
	@echo "Binaries installed"

# Full deployment
deploy: all
	@echo "=== Full Elxnk Deployment ==="
	@echo ""
	@echo "[1/5] Installing binaries..."
	@$(MAKE) install -s
	@echo ""
	@echo "[2/5] Installing configurations..."
	ssh root@$(HOST) "mkdir -p $(INSTALL_ETC)"
	scp config/elxnk.conf root@$(HOST):$(INSTALL_ETC)/
	scp genie_lamp/ui.conf root@$(HOST):$(INSTALL_ETC)/genie_ui.conf
	@echo ""
	@echo "[3/5] Installing systemd service..."
	scp config/elxnk.service root@$(HOST):$(INSTALL_SYSTEMD)/
	ssh root@$(HOST) "systemctl daemon-reload"
	@echo ""
	@echo "[4/5] Enabling and starting service..."
	ssh root@$(HOST) "systemctl enable elxnk"
	ssh root@$(HOST) "systemctl restart elxnk"
	@echo ""
	@echo "[5/5] Verifying..."
	@sleep 2
	@$(MAKE) status -s
	@echo ""
	@echo "=== Deployment Complete ==="

# Service management
start:
	ssh root@$(HOST) "systemctl start elxnk"
	@echo "Elxnk started"

stop:
	ssh root@$(HOST) "systemctl stop elxnk"
	@echo "Elxnk stopped"

restart:
	ssh root@$(HOST) "systemctl restart elxnk"
	@echo "Elxnk restarted"

status:
	@echo "=== Service Status ==="
	@ssh root@$(HOST) "systemctl is-active elxnk && echo '✓ Running' || echo '✗ Stopped'"
	@echo ""
	@echo "=== Processes ==="
	@ssh root@$(HOST) "ps | grep -E 'elxnk|lamp|genie' | grep -v grep" || echo "No processes running"
	@echo ""
	@echo "=== Recent Logs ==="
	@ssh root@$(HOST) "tail -10 /tmp/elxnk.log 2>/dev/null || echo 'No logs yet'"

logs:
	ssh root@$(HOST) "tail -f /tmp/elxnk.log"

# Uninstall everything
uninstall:
	@echo "Uninstalling Elxnk..."
	ssh root@$(HOST) "systemctl stop elxnk 2>/dev/null || true"
	ssh root@$(HOST) "systemctl disable elxnk 2>/dev/null || true"
	ssh root@$(HOST) "rm -f $(INSTALL_BIN)/{elxnk,lamp,genie_lamp}"
	ssh root@$(HOST) "rm -f $(INSTALL_ETC)/{elxnk.conf,genie_ui.conf}"
	ssh root@$(HOST) "rm -f $(INSTALL_SYSTEMD)/elxnk.service"
	ssh root@$(HOST) "systemctl daemon-reload"
	ssh root@$(HOST) "rm -f /tmp/elxnk.log /tmp/elxnk.pid /tmp/elxnk_lamp.pipe"
	@echo "Uninstalled"

# Help
help:
	@echo "Elxnk Unified Build System"
	@echo ""
	@echo "Build Targets:"
	@echo "  all          Build all components (default)"
	@echo "  library      Generate component library header"
	@echo "  elxnk        Build elxnk controller only"
	@echo "  genie        Build genie_lamp only"
	@echo "  lamp         Build lamp only"
	@echo "  clean        Remove all build artifacts"
	@echo ""
	@echo "Deployment:"
	@echo "  deploy       Full deployment to reMarkable 2"
	@echo "  install      Install binaries only"
	@echo "  uninstall    Remove everything"
	@echo ""
	@echo "Service Control:"
	@echo "  start        Start elxnk service"
	@echo "  stop         Stop elxnk service"
	@echo "  restart      Restart elxnk service"
	@echo "  status       Show service status"
	@echo "  logs         View logs (tail -f)"
	@echo ""
	@echo "Variables:"
	@echo "  HOST         reMarkable IP (default: 10.11.99.1)"
	@echo ""
	@echo "Examples:"
	@echo "  make all              # Build everything"
	@echo "  make deploy           # Deploy to reMarkable"
	@echo "  make HOST=192.168.1.2 deploy  # Custom IP"
	@echo "  make status           # Check if running"
	@echo ""
