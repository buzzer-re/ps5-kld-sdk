#!/bin/bash

set -e

INSTALL_DIR="/opt/ps5-kld-sdk"
INCLUDE_DIR="$INSTALL_DIR/include"
LIB_DIR="$INSTALL_DIR/lib"
LD_DIR="$INSTALL_DIR/ld"
FREEBSD_HEADERS_DIR="freebsd-headers"
PS5KLD_INCLUDE_DIR="include/ps5kld"
BUILD_DIR="build"
LINKER_SCRIPT="linker.x"

echo "Starting PS5 KLD SDK installation..."
make

# Create main install directory if it doesn't exist
if [ ! -d "$INSTALL_DIR" ]; then
    echo "Creating install directory at $INSTALL_DIR"
    sudo mkdir -p "$INSTALL_DIR"
    sudo mkdir -p "$INCLUDE_DIR"
    sudo mkdir -p "$LIB_DIR"
fi


# Copy freebsd-headers
if [ -d "$FREEBSD_HEADERS_DIR" ]; then
    echo "Copying $FREEBSD_HEADERS_DIR to $INCLUDE_DIR"
    sudo cp -r "$FREEBSD_HEADERS_DIR" "$INCLUDE_DIR/"
else
    echo "Error: Directory $FREEBSD_HEADERS_DIR does not exist"
    exit 1
fi

# Copy include/ps5kld
if [ -d "$PS5KLD_INCLUDE_DIR" ]; then
    echo "Copying $PS5KLD_INCLUDE_DIR to $INCLUDE_DIR"
    sudo cp -r "$PS5KLD_INCLUDE_DIR" "$INCLUDE_DIR/"
else
    echo "Error: Directory $PS5KLD_INCLUDE_DIR does not exist"
    exit 1
fi

# Create lib directory
if [ ! -d "$LIB_DIR" ]; then
    echo "Creating lib directory at $LIB_DIR"
    sudo mkdir -p "$LIB_DIR"
fi

# Copy .o files
if compgen -G "$BUILD_DIR/*.o" > /dev/null; then
    echo "Copying object files to $LIB_DIR"
    sudo cp "$BUILD_DIR"/*.o "$LIB_DIR/"
else
    echo "Warning: No object files found in $BUILD_DIR"
fi

# Create ld directory
if [ ! -d "$LD_DIR" ]; then
    echo "Creating ld directory at $LD_DIR"
    sudo mkdir -p "$LD_DIR"
fi

# Copy linker script
if [ -f "$LINKER_SCRIPT" ]; then
    echo "Copying $LINKER_SCRIPT to $LD_DIR"
    sudo cp "$LINKER_SCRIPT" "$LD_DIR/"
else
    echo "Error: $LINKER_SCRIPT not found"
    exit 1
fi

echo "Installation completed successfully."
