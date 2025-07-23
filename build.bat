@echo off
echo Building Huffman Compression Utility...

:: Set compiler and flags
set CXX=g++
set CXXFLAGS=-std=c++11 -Wall -Wextra -Iinclude
set TARGET=huff.exe

:: Check if compiler exists
where %CXX% >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: %CXX% compiler not found. Please install MinGW-w64 or similar.
    echo Make sure g++ is in your PATH.
    pause
    exit /b 1
)

echo Compiler found: %CXX%
echo Compiling source files...

:: Compile all source files in one command
%CXX% %CXXFLAGS% -O2 -o %TARGET% ^
    main.cpp ^
    src/CommandLineOptions.cpp ^
    src/HuffmanException.cpp ^
    src/HuffmanNode.cpp ^
    src/ArchiveStructures.cpp ^
    src/HuffmanAlgorithm.cpp

if %errorlevel% equ 0 (
    echo.
    echo Build successful! Executable created: %TARGET%
    echo.
    echo Usage examples:
    echo   %TARGET% -e file1.txt file2.txt -o archive.huf
    echo   %TARGET% -d archive.huf
    echo   %TARGET% -i archive.huf -v
    echo.
) else (
    echo.
    echo Build failed! Check error messages above.
    echo.
)

pause
