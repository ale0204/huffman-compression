@echo off
echo =====================================================
echo Huffman Compression Tool - Deployment Script
echo =====================================================
echo.

REM Check if we're in the correct directory
if not exist "main.cpp" (
    echo Error: main.cpp not found. Please run this script from the project root directory.
    pause
    exit /b 1
)

REM Check if Node.js is installed
node --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: Node.js is not installed or not in PATH.
    echo Please install Node.js 14 or higher from https://nodejs.org/
    pause
    exit /b 1
)

echo [1/4] Checking C++ compilation...

REM Check if huff.exe exists and is newer than source files
set NEED_COMPILE=0

if not exist "huff.exe" (
    echo huff.exe not found - will compile
    set NEED_COMPILE=1
) else (
    REM Check if any source files are newer than huff.exe
    for %%f in (main.cpp src\*.cpp include\*.h) do (
        if "%%f" neq "" (
            forfiles /p . /m "%%f" /c "cmd /c if @fdate gtr %date% set NEED_COMPILE=1" 2>nul
        )
    )
)

if %NEED_COMPILE%==1 (
    echo Compiling C++ source code...
    call build.bat
    if %errorlevel% neq 0 (
        echo Error: C++ compilation failed.
        pause
        exit /b 1
    )
    echo C++ compilation completed successfully.
) else (
    echo C++ executable is up to date.
)

echo.
echo [2/4] Setting up web interface...

REM Navigate to web-ui directory
cd web-ui

REM Check if package.json exists
if not exist "package.json" (
    echo Error: package.json not found in web-ui directory.
    pause
    exit /b 1
)

echo [3/4] Installing Node.js dependencies...

REM Install dependencies if node_modules doesn't exist or package.json is newer
if not exist "node_modules" (
    echo Installing dependencies...
    npm install
    if %errorlevel% neq 0 (
        echo Error: Failed to install Node.js dependencies.
        pause
        exit /b 1
    )
) else (
    echo Dependencies already installed.
)

echo.
echo [4/4] Starting web server...
echo.
echo =====================================================
echo Server will start at: http://localhost:50204
echo Press Ctrl+C to stop the server
echo =====================================================
echo.

REM Start the server
npm start
