# Lamp Makefile for reMarkable 2
# Standalone drawing engine with geometry functions

CXX = arm-linux-gnueabihf-g++
CXXFLAGS = -O2 -std=c++11 -Wall -Wextra
TARGET = lamp
SRC = main.cpp

# reMarkable 2 deployment
HOST ?= 10.11.99.1
INSTALL_PATH = /opt/bin

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

# Deploy to reMarkable 2
deploy: $(TARGET)
	scp $(TARGET) root@$(HOST):$(INSTALL_PATH)/$(TARGET)
	ssh root@$(HOST) "chmod +x $(INSTALL_PATH)/$(TARGET)"
	@echo "✓ Lamp deployed to $(HOST):$(INSTALL_PATH)/$(TARGET)"

# Test lamp connection
test:
	@echo "Testing lamp on reMarkable 2..."
	@echo "pen down 500 500\npen move 600 600\npen up" | ssh root@$(HOST) $(INSTALL_PATH)/$(TARGET)
	@echo "✓ Test complete - check screen for diagonal line"

.PHONY: all clean deploy test
