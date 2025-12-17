# Elxnk Controller Build System
# Builds the main integration binary for reMarkable 2

# Compiler and flags
CXX = arm-linux-gnueabihf-g++
CXXFLAGS = -O2 -std=c++11 -Wall -Wextra
TARGET_BINARY = elxnk

# Build directory
BUILD_DIR = ../../build
BIN_DIR = $(BUILD_DIR)/bin

# Source and object files
SRCS = elxnk_main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = $(BIN_DIR)/$(TARGET_BINARY)

# Installation paths on reMarkable
INSTALL_BIN = /opt/bin
INSTALL_ETC = /opt/etc
INSTALL_SYSTEMD = /etc/systemd/system

# Deployment configuration
HOST ?= 10.11.99.1
DEPLOY_DIR = /home/root/lamp-v2

.PHONY: all clean install deploy test

all: $(TARGET)

# Create build directories
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Build elxnk binary
$(TARGET): $(SRCS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<
	@echo "Built: $(TARGET)"

# Clean build artifacts
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
	@echo "Cleaned build artifacts"

# Deploy to reMarkable 2
deploy: $(TARGET)
	@echo "Deploying Elxnk to reMarkable 2..."
	ssh root@$(HOST) "mkdir -p $(INSTALL_BIN) $(INSTALL_ETC)"
	scp $(TARGET) root@$(HOST):$(INSTALL_BIN)/$(TARGET_BINARY)
	ssh root@$(HOST) "chmod +x $(INSTALL_BIN)/$(TARGET_BINARY)"
	@echo "Deployed successfully!"

# Install systemd service
install-service: deploy
	@echo "Installing Elxnk systemd service..."
	scp ../config/elxnk.service root@$(HOST):$(INSTALL_SYSTEMD)/
	ssh root@$(HOST) "systemctl daemon-reload"
	@echo "Service installed. Use 'make enable' to enable it."

# Enable and start service
enable:
	@echo "Enabling Elxnk service..."
	ssh root@$(HOST) "systemctl enable elxnk"
	ssh root@$(HOST) "systemctl start elxnk"
	@echo "Service enabled and started"

# Start service
start:
	ssh root@$(HOST) "systemctl start elxnk"

# Stop service
stop:
	ssh root@$(HOST) "systemctl stop elxnk"

# Restart service
restart:
	ssh root@$(HOST) "systemctl restart elxnk"

# Service status
status:
	ssh root@$(HOST) "systemctl status elxnk"

# View logs
logs:
	ssh root@$(HOST) "tail -f /tmp/elxnk.log"

# Full deployment with all components
deploy-full: $(TARGET)
	@echo "=== Full Elxnk Deployment ==="
	@echo "1. Building all components..."
	cd ../genie_lamp && $(MAKE) all
	cd ../../resources/rmkit && $(MAKE) lamp
	@echo ""
	@echo "2. Deploying binaries..."
	ssh root@$(HOST) "mkdir -p $(INSTALL_BIN) $(INSTALL_ETC) $(DEPLOY_DIR)"
	scp $(TARGET) root@$(HOST):$(INSTALL_BIN)/$(TARGET_BINARY)
	scp ../genie_lamp/genie_lamp root@$(HOST):$(INSTALL_BIN)/
	scp ../../resources/rmkit/src/build/lamp root@$(HOST):$(INSTALL_BIN)/
	@echo ""
	@echo "3. Deploying configurations..."
	scp ../genie_lamp/ui.conf root@$(HOST):$(INSTALL_ETC)/genie_ui.conf
	scp ../config/elxnk.conf root@$(HOST):$(INSTALL_ETC)/
	@echo ""
	@echo "4. Deploying scripts and assets..."
	ssh root@$(HOST) "mkdir -p $(DEPLOY_DIR)/scripts $(DEPLOY_DIR)/assets"
	scp ../../scripts/*.sh root@$(HOST):$(DEPLOY_DIR)/scripts/
	scp -r ../../assets/* root@$(HOST):$(DEPLOY_DIR)/assets/
	@echo ""
	@echo "5. Setting permissions..."
	ssh root@$(HOST) "chmod +x $(INSTALL_BIN)/* $(DEPLOY_DIR)/scripts/*.sh"
	@echo ""
	@echo "6. Installing systemd service..."
	scp ../config/elxnk.service root@$(HOST):$(INSTALL_SYSTEMD)/
	ssh root@$(HOST) "systemctl daemon-reload"
	@echo ""
	@echo "=== Deployment Complete ==="
	@echo "Run 'make enable' to start Elxnk as a service"
	@echo "Or run manually: ssh root@$(HOST) /opt/bin/elxnk"

# Test deployment (run without service)
test-run:
	ssh root@$(HOST) "/opt/bin/elxnk"

# Uninstall everything
uninstall:
	@echo "Uninstalling Elxnk..."
	ssh root@$(HOST) "systemctl stop elxnk 2>/dev/null || true"
	ssh root@$(HOST) "systemctl disable elxnk 2>/dev/null || true"
	ssh root@$(HOST) "rm -f $(INSTALL_BIN)/elxnk $(INSTALL_BIN)/lamp $(INSTALL_BIN)/genie_lamp"
	ssh root@$(HOST) "rm -f $(INSTALL_ETC)/elxnk.conf $(INSTALL_ETC)/genie_ui.conf"
	ssh root@$(HOST) "rm -f $(INSTALL_SYSTEMD)/elxnk.service"
	ssh root@$(HOST) "systemctl daemon-reload"
	ssh root@$(HOST) "rm -rf $(DEPLOY_DIR)"
	@echo "Uninstalled successfully"

# Help
help:
	@echo "Elxnk Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all           - Build elxnk binary (default)"
	@echo "  clean         - Remove build artifacts"
	@echo "  deploy        - Deploy elxnk binary to reMarkable"
	@echo "  deploy-full   - Build and deploy everything (recommended)"
	@echo "  install-service - Install systemd service"
	@echo "  enable        - Enable and start service"
	@echo "  start         - Start elxnk service"
	@echo "  stop          - Stop elxnk service"
	@echo "  restart       - Restart elxnk service"
	@echo "  status        - Show service status"
	@echo "  logs          - View elxnk logs (tail -f)"
	@echo "  test-run      - Run elxnk manually (not as service)"
	@echo "  uninstall     - Remove everything from reMarkable"
	@echo "  help          - Show this help"
	@echo ""
	@echo "Variables:"
	@echo "  HOST          - reMarkable IP (default: 10.11.99.1)"
	@echo ""
	@echo "Example: make deploy-full HOST=10.11.99.1"
