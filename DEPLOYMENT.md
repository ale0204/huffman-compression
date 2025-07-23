# Huffman Compression Tool - Deployment Guide

This guide will help you deploy the Huffman Compression Tool with its web interface on any system.

## Quick Start (Recommended)

### Windows
```bash
# Clone the repository
git clone https://github.com/ale0204/huffman-compression.git
cd huffman-compression

# Run the deployment script
deploy.bat
```

### Linux/macOS/WSL
```bash
# Clone the repository
git clone https://github.com/ale0204/huffman-compression.git
cd huffman-compression

# Make the script executable and run it
chmod +x deploy.sh
./deploy.sh
```

The deployment script will automatically:
1. ✅ Check if C++ code needs compilation
2. ✅ Compile the Huffman algorithm if source files changed
3. ✅ Install Node.js dependencies for the web interface
4. ✅ Start the web server at `http://localhost:50204`

## Manual Setup

If you prefer to set up manually or the automated script doesn't work:

### Prerequisites
- **Node.js 14+**: Download from [nodejs.org](https://nodejs.org/)
- **C++ Compiler**: GCC, Clang, or MSVC

### Step 1: Compile C++ Code
```bash
# Windows (using provided batch file)
build.bat

# Linux/macOS (using Make)
make

# Manual compilation
g++ -std=c++17 -I include src/*.cpp main.cpp -o huff
```

### Step 2: Set Up Web Interface
```bash
cd web-ui
npm install
npm start
```

### Step 3: Access the Application
Open your browser and go to: **http://localhost:50204**

## Project Structure

```
huffman-compression/
├── deploy.bat              # Windows deployment script
├── deploy.sh               # Unix deployment script  
├── build.bat               # C++ build script
├── main.cpp                # Main C++ source
├── include/                # C++ header files
├── src/                    # C++ implementation files
└── web-ui/                 # Node.js web interface
    ├── package.json        # Dependencies
    ├── server.js          # Express server
    ├── public/            # Static web files
    └── README.md          # Detailed web UI documentation
```

## Features

- 🎯 **Drag & Drop Interface**: Easy file upload
- 📊 **Real-time Statistics**: Compression metrics and efficiency
- 📈 **Interactive Frequency Table**: Sortable character analysis
- 🎨 **Modern UI**: Responsive design with animations
- 🔧 **Educational**: Information theory concepts visualization

## Troubleshooting

### C++ Compilation Issues
- **Windows**: Ensure you have Visual Studio Build Tools or MinGW
- **Linux**: Install `build-essential`: `sudo apt install build-essential`
- **macOS**: Install Xcode Command Line Tools: `xcode-select --install`

### Node.js Issues
- **Version**: Ensure Node.js 14 or higher is installed
- **Permissions**: On some systems, you may need to use `sudo npm install`
- **Port**: If port 50204 is busy, modify `web-ui/server.js` to use a different port

### File Access Issues
- **Permissions**: Ensure the web server has read/write access to the project directory
- **Firewall**: Check if localhost connections are blocked

## Production Deployment

For production deployment on a remote server:

1. **Security**: Change the default port and add authentication
2. **Process Management**: Use PM2 or similar process manager
3. **Reverse Proxy**: Set up Nginx or Apache for better performance
4. **HTTPS**: Add SSL certificates for secure connections

## Educational Use

This tool is designed for educational purposes in computer science courses covering:
- Information Theory and Compression Algorithms
- Data Structures and Algorithm Analysis  
- Web Development and Full-Stack Applications
- Binary File Processing and Bit Manipulation

---

**Course**: Calculatoare Numerice (Numerical Computing)  
**Semester**: 2, 2024-2025  
**Author**: Alexandra
