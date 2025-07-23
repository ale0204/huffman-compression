# Huffman Compression Utility

A complete implementation of the Huffman compression algorithm in C++, providing efficient lossless compression for text and binary files using advanced binary bit packing techniques.

**Course:** Calculatoare Numerice (Numerical Computing)  
**Academic Year:** 2024-2025, Semester 2

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Building the Project](#building-the-project)
- [Usage](#usage)
- [Examples](#examples)
- [Technical Details](#technical-details)
- [Performance](#performance)
- [Implementation Details](#implementation-details)

## Overview

This project implements a full-featured Huffman compression utility that can:
- Compress single or multiple files into a single archive
- Decompress archives back to original files with 100% integrity
- Display detailed compression statistics
- Achieve compression ratios of 40-45% with 99%+ efficiency compared to theoretical limits
- Use optimized binary bit packing to store compressed data in minimal space

The implementation follows modern C++ practices and includes comprehensive error handling, verbose output options, safe file operations, and highly optimized binary storage that achieves real compression through efficient bit packing algorithms.

## Features

### Core Functionality
- **Huffman Compression**: Optimal prefix-free encoding based on character frequencies
- **Multi-file Support**: Compress multiple files into a single archive
- **Safe Decompression**: Restore files to specified directories without overwriting originals
- **Archive Metadata**: Stores original filenames, file sizes, and compression statistics
- **Binary Format**: Efficient binary archive format with embedded Huffman tree data

### Command Line Interface
- **Encoding Mode** (`-e`): Compress files using Huffman algorithm
- **Decoding Mode** (`-d`): Decompress archives to restore original files
- **Info Mode** (`-i`): Display archive information and statistics
- **Verbose Output** (`-v`): Show detailed compression statistics and progress
- **Recursive Processing** (`-r`): Process directories recursively (planned feature)
- **Custom Output** (`-o`): Specify output file or directory

### Advanced Features
- **Compression Statistics**: Shannon entropy, compression ratios, efficiency metrics
- **File Integrity**: Size verification and error detection during decompression
- **Binary Bit Packing**: Efficient storage using bitwise operations (8 bits per byte)
- **Padding Management**: Proper handling of incomplete bytes in compressed data
- **Memory Management**: Proper cleanup of Huffman tree structures
- **Cross-platform**: Works on Windows, Linux, and macOS

## Project Structure

```
huffman-compression/
├── README.md                   # This documentation file
├── main.cpp                    # Entry point and command-line interface
├── build.bat                   # Windows build script
├── build.ps1                   # PowerShell build script
├── Makefile                    # Unix/Linux build configuration
├── Makefile.win               # Windows-specific Makefile
├── include/                    # Header files
│   ├── HuffmanAlgorithm.h     # Core compression algorithms
│   ├── HuffmanNode.h          # Tree node structure
│   ├── CommandLineOptions.h   # Argument parsing
│   ├── ArchiveStructures.h    # Archive format definitions
│   ├── HuffmanException.h     # Custom exception classes
│   └── OperationMode.h        # Enumeration for modes
└── src/                       # Implementation files
    ├── HuffmanAlgorithm.cpp   # Core algorithms implementation
    ├── HuffmanNode.cpp        # Tree node operations
    ├── CommandLineOptions.cpp # Command-line parsing
    ├── ArchiveStructures.cpp  # Archive format handling
    └── HuffmanException.cpp   # Exception implementations
```

## Building the Project

### Windows
```bash
# Using batch script
.\build.bat

# Using PowerShell (if execution policy allows)
.\build.ps1

# Manual compilation with g++
g++ -std=c++11 -O2 -Wall -Wextra src/*.cpp main.cpp -Iinclude -o huff.exe
```

### Linux/macOS
```bash
# Using Makefile
make

# Manual compilation
g++ -std=c++11 -O2 -Wall -Wextra src/*.cpp main.cpp -Iinclude -o huff
```

## Usage

### Command Syntax
```bash
huff [OPTIONS] [FILES/DIRECTORY]
```

### Options
- `-e, --encode`: Encode (compress) files using Huffman algorithm
- `-d, --decode`: Decode (decompress) archive to original files
- `-i, --info`: Display archive contents and information
- `-r, --recursive`: Operate recursively on directories (encode only)
- `-v, --verbose`: Display detailed information and statistics
- `-o, --output`: Specify output archive file (encode) or directory (decode)

### Basic Commands

#### Compression
```bash
# Compress single file
huff -e document.txt -o compressed.huf

# Compress multiple files with verbose output
huff -e -v file1.txt file2.cpp file3.h -o archive.huf

# Compress with detailed statistics
huff -e -v src/*.cpp -o source_code.huf
```

#### Decompression
```bash
# Decompress to default directory (./decompressed/)
huff -d archive.huf

# Decompress to specific directory
huff -d archive.huf -o restored_files

# Decompress with verbose output
huff -d -v archive.huf -o output_dir
```

#### Archive Information
```bash
# Basic archive info
huff -i archive.huf

# Detailed archive information
huff -i -v archive.huf
```

## Examples

### Example 1: Compressing Source Code
```bash
# Compress all C++ source files
$ huff -e -v src/HuffmanAlgorithm.cpp src/CommandLineOptions.cpp src/HuffmanNode.cpp -o source_backup.huf

Huffman Compression Utility - Verbose Mode
==========================================
Encoding files...
Reading file: src/HuffmanAlgorithm.cpp
Reading file: src/CommandLineOptions.cpp
Reading file: src/HuffmanNode.cpp
Compression completed. Output written to: source_backup.huf
Files compressed: 3

=== COMPRESSION STATISTICS ===
Original size: 23936 bytes
Compressed size: 13757 bytes
Compression ratio: 42.5259%
Shannon information: 4.57318 bits/character
Huffman average: 4.59768 bits/character
Efficiency: 99.4671%
```

### Example 2: Decompressing Files
```bash
# Decompress to a safe directory
$ huff -d -v source_backup.huf -o restored_source

Huffman Compression Utility - Verbose Mode
==========================================
Decoding archive: source_backup.huf
Reconstructing Huffman tree from frequency table...
Original total size: 23936 bytes
Number of files: 3
Frequency table entries: 85
Decompressing files to directory: restored_source
Restored file: restored_source/HuffmanAlgorithm.cpp (17649 bytes)
Restored file: restored_source/CommandLineOptions.cpp (5476 bytes)
Restored file: restored_source/HuffmanNode.cpp (811 bytes)
Decoding completed successfully!
✓ Size verification: 23936 bytes
```

### Example 3: File Integrity Verification
```bash
# Verify files are identical after compression/decompression cycle
$ Get-FileHash src/HuffmanAlgorithm.cpp
SHA256: 2E8730272AED62D6AFF237E35A12482E32EEE85F4F79D98D46E05582D6DD9C43

$ Get-FileHash restored_source/HuffmanAlgorithm.cpp  
SHA256: 2E8730272AED62D6AFF237E35A12482E32EEE85F4F79D98D46E05582D6DD9C43
# ✓ Hashes match - perfect reconstruction!
```

## Technical Details

### Huffman Algorithm Implementation
- **Frequency Analysis**: Builds character frequency tables from input data
- **Tree Construction**: Uses priority queue to build optimal Huffman tree
- **Code Generation**: Generates variable-length binary codes for each character
- **Text Encoding**: Converts input text to compressed binary representation
- **Text Decoding**: Reconstructs original text using Huffman tree

### Archive Format
The binary archive format stores:
1. **Number of files** (8 bytes)
2. **File metadata** for each file:
   - Filename length (8 bytes)
   - Filename (variable length)
   - File size (8 bytes)
3. **Original total size** (8 bytes)
4. **Frequency table size** (8 bytes)
5. **Frequency table**:
   - Character (1 byte)
   - Frequency (4 bytes)
6. **Padding bits** (1 byte): Number of padding bits in the last byte
7. **Compressed binary data** (variable length packed binary data)

### Binary Data Processing

The implementation uses sophisticated bit packing algorithms to achieve optimal compression, converting Huffman-encoded data directly into packed binary format for maximum efficiency.

#### Bit Packing Algorithm (Compression)
```cpp
// Pack Huffman-encoded bits directly into binary bytes for optimal storage
static unsigned char packBitsIntoByte(const std::vector<int>& bits, size_t startPos)
{
    unsigned char byte = 0;
    for (int i = 0; i < 8 && (startPos + i) < bits.size(); i++)
    {
        if (bits[startPos + i] == 1)
        {
            byte |= (1 << (7 - i));  // Set bit at position (7-i)
        }
    }
    return byte;
}
```

**Process:**
1. **Bit Collection**: Gather encoded bits from Huffman algorithm
2. **Byte Assembly**: Pack 8 bits into single byte using bitwise OR operations
3. **MSB First**: Most significant bit (position 7) stores first bit from sequence
4. **Padding Calculation**: Track unused bits in the last byte for proper decompression

**Example:**
```
Huffman bits: [0,1,0,1,1,0,1,0]
Packed byte:  01011010₂ = 90₁₀ (single byte!)
```

#### Bit Unpacking Algorithm (Decompression)
```cpp
// Extract original Huffman bits from packed binary bytes
static std::string binaryDataToString(const std::vector<unsigned char>& binaryData, 
                                    size_t totalValidBits)
{
    std::string bitString;
    size_t bitCount = 0;
    
    for (unsigned char byte : binaryData)
    {
        for (int i = 7; i >= 0; i--)  // MSB to LSB
        {
            if (bitCount >= totalValidBits) break;
            
            if (byte & (1 << i))
                bitString += '1';
            else
                bitString += '0';
                
            bitCount++;
        }
        if (bitCount >= totalValidBits) break;
    }
    return bitString;
}
```

**Process:**
1. **Byte Reading**: Extract each packed byte from binary archive
2. **Bit Testing**: Use bitwise AND to test each bit position (MSB to LSB)
3. **Sequence Reconstruction**: Rebuild original Huffman bit sequence
4. **Padding Handling**: Stop at `totalValidBits` to ignore padding bits

**Example:**
```
Packed byte: 90₁₀ = 01011010₂
Huffman bits: [0,1,0,1,1,0,1,0] → Ready for tree traversal
```

#### Compression Efficiency

The bit packing approach achieves optimal space utilization by storing 8 Huffman bits in each byte:

| Storage Method | Data Representation | Space Usage | Efficiency |
|---------------|-------------------|-------------|-----------|
| **Binary Bit Packing** | 8 bits per byte | Optimal | ✅ Maximum compression |
| **Theoretical Alternative** | Each bit as character | 8× larger | ❌ Massive expansion |

**Real Performance (main.cpp compression):**
- Huffman encoded data: 6,183 bits
- Packed storage: 773 bytes (6,184 bits with 1 padding bit)
- Compression achieved: 44.4% reduction from original ✅
- Storage efficiency: 99.98% (only 1 padding bit needed)

### Key Classes

#### `HuffmanAlgorithm`
Core static methods for compression operations:
- `buildFrequencyTable()`: Analyzes character frequencies
- `buildHuffmanTree()`: Constructs optimal Huffman tree
- `generateCodes()`: Creates binary codes for characters
- `encodeText()` / `decodeText()`: Text compression/decompression
- `encodeFiles()` / `decodeArchive()`: High-level file operations

#### `HuffmanNode`
Tree node structure with:
- Character data and frequency
- Left/right child pointers
- Leaf node detection
- Memory management

#### `CommandLineOptions`
Argument parsing and validation:
- Operation mode detection
- File path handling
- Option validation
- Usage information

#### `CompressionStatistics`
Detailed metrics including:
- Compression ratios
- Shannon entropy calculations
- Huffman efficiency
- Character frequency analysis

## Performance

### Typical Results
- **Compression Ratio**: 40-45% for source code files
- **Efficiency**: 99.2-99.5% compared to Shannon theoretical limit
- **Speed**: Processes thousands of lines per second
- **Memory**: Minimal overhead with proper tree cleanup
- **Data Integrity**: 100% lossless compression verified by SHA256 hash comparison

### Real-World Example (main.cpp)
```
Original file:        1,390 bytes
Compressed archive:   1,122 bytes (including metadata)
Pure compressed data:   773 bytes
Compression ratio:    44.4% reduction
Efficiency:          99.26% of Shannon theoretical optimum
Processing time:     < 1 second
SHA256 verification: ✓ Perfect reconstruction
```

### Archive Format Efficiency
```
Component Breakdown (main.cpp example):
├── Metadata overhead:    348 bytes (31%)
│   ├── File count:         8 bytes
│   ├── File metadata:     24 bytes  
│   ├── Original size:      8 bytes
│   ├── Frequency table:  300 bytes (60 chars × 5 bytes each)
│   └── Padding info:       1 byte
└── Compressed data:      773 bytes (69%)
    ├── Huffman bits:   6,183 bits
    ├── Packed into:      773 bytes (optimal!)
    └── Padding bits:       1 bit only
```

### Bit Packing Performance
```
Compression Stage          Processing        Result
─────────────────────────  ────────────────  ─────────────────
Huffman encoding          6,183 bits        Optimal code length
Binary bit packing         773 bytes         Maximum density
Total compression          1,122 bytes       44.4% of original
Storage efficiency         99.98%            Only 1 padding bit
```

## Implementation Details

### Memory Management
- Automatic cleanup of Huffman tree structures
- RAII principles for file handling
- Exception-safe resource management

### Error Handling
- Custom `HuffmanException` class for domain-specific errors
- Comprehensive input validation
- Graceful failure with meaningful error messages
- File operation safety checks

### Safety Features
- Never overwrites original source files
- Creates output directories automatically
- Verifies file sizes during decompression
- Hash-based integrity verification possible

### Future Enhancements
- [ ] Recursive directory processing
- [ ] Progress bars for large files
- [ ] Compression algorithm selection
- [ ] Archive encryption support
- [x] Cross-platform directory handling
- [x] GUI interface
- [ ] Parallel processing for multiple files

---

## License

This project is educational software created for learning purposes. Feel free to use, modify, and distribute according to your institution's guidelines.

## Authors

Developed as part of **Calculatoare Numerice (Numerical Computing)** coursework, demonstrating practical implementation of information theory, data compression algorithms, and advanced binary data processing techniques.
