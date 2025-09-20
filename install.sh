#!/bin/bash

INSTALL_DIR="/opt/ps5-kld-sdk"
INCLUDE_DIR="$INSTALL_DIR/include"
LIB_DIR="$INSTALL_DIR/lib"
LD_DIR="$INSTALL_DIR/ld"
TEMPLATE_PROJ="$INSTALL_DIR/sample"
FREEBSD_HEADERS_DIR="freebsd-headers"
PS5KLD_INCLUDE_DIR="include/ps5kld"
BUILD_DIR="build"
LINKER_SCRIPT="linker.x"
SAMPLE_PROJECT=sample/hello_world

echo "Starting PS5 KLD SDK installation..."
make

# Create main install directory if it doesn't exist
if [ ! -d "$INSTALL_DIR" ]; then
    echo "Creating install directory at $INSTALL_DIR"
    mkdir -p "$INSTALL_DIR"
    mkdir -p "$INCLUDE_DIR"
    mkdir -p "$LIB_DIR"
    mkdir -p "$LD_DIR"
    mkdir -p "$TEMPLATE_PROJ"
fi

echo "Copying headers..."
#cp -r "$FREEBSD_HEADERS_DIR" "$INCLUDE_DIR/"
#cp -r "$PS5KLD_INCLUDE_DIR" "$INCLUDE_DIR/"
echo "Copying objects..."
cp "$BUILD_DIR"/*.o "$LIB_DIR/"
cp "$LINKER_SCRIPT" "$LD_DIR/"
cp -r "$SAMPLE_PROJECT" "$TEMPLATE_PROJ"

echo "Done"
