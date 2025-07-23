#!/bin/bash

echo "====================================================="
echo "Huffman Compression Tool - Deployment Script"
echo "====================================================="
echo

# Check if we're in the correct directory
if [ ! -f "main.cpp" ]; then
    echo "Error: main.cpp not found. Please run this script from the project root directory."
    exit 1
fi

# Check if Node.js is installed
if ! command -v node &> /dev/null; then
    echo "Error: Node.js is not installed or not in PATH."
    echo "Please install Node.js 14 or higher from https://nodejs.org/"
    exit 1
fi

echo "[1/4] Checking C++ compilation..."

# Check if we need to compile (huff executable doesn't exist or source files are newer)
NEED_COMPILE=0

if [ ! -f "huff" ] && [ ! -f "huff.exe" ]; then
    echo "Huffman executable not found - will compile"
    NEED_COMPILE=1
else
    # Check if any source files are newer than the executable
    if [ -f "huff" ]; then
        EXECUTABLE="huff"
    else
        EXECUTABLE="huff.exe"
    fi
    
    for file in main.cpp src/*.cpp include/*.h; do
        if [ -f "$file" ] && [ "$file" -nt "$EXECUTABLE" ]; then
            echo "Source file $file is newer than executable - will recompile"
            NEED_COMPILE=1
            break
        fi
    done
fi

if [ $NEED_COMPILE -eq 1 ]; then
    echo "Compiling C++ source code..."
    
    # Try different compilation methods
    if [ -f "build.bat" ]; then
        # On Windows with WSL or Git Bash
        cmd //c build.bat
    elif [ -f "Makefile" ]; then
        make
    else
        # Direct compilation
        echo "Using direct g++ compilation..."
        g++ -std=c++17 -I include src/*.cpp main.cpp -o huff
    fi
    
    if [ $? -ne 0 ]; then
        echo "Error: C++ compilation failed."
        exit 1
    fi
    echo "C++ compilation completed successfully."
else
    echo "C++ executable is up to date."
fi

echo
echo "[2/4] Setting up web interface..."

# Navigate to web-ui directory
cd web-ui

# Check if package.json exists
if [ ! -f "package.json" ]; then
    echo "Error: package.json not found in web-ui directory."
    exit 1
fi

echo "[3/4] Installing Node.js dependencies..."

# Install dependencies if node_modules doesn't exist or package.json is newer
if [ ! -d "node_modules" ] || [ "package.json" -nt "node_modules" ]; then
    echo "Installing dependencies..."
    npm install
    if [ $? -ne 0 ]; then
        echo "Error: Failed to install Node.js dependencies."
        exit 1
    fi
else
    echo "Dependencies already installed."
fi

echo
echo "[4/4] Starting web server..."
echo
echo "====================================================="
echo "Server will start at: http://localhost:50204"
echo "Press Ctrl+C to stop the server"
echo "====================================================="
echo

# Start the server
npm start
