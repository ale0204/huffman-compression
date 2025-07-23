#pragma once

/**
 * @brief Enumeration of allowed operations for the Huffman compression tool
 * 
 * This enum defines the three main modes of operation that the compression
 * utility can perform on files and directories.
 */
enum class OperationMode {
    /**
     * @brief No operation mode specified
     * 
     * Default value when no operation mode has been set from command line.
     * Indicates an error condition that should be caught during validation.
     */
    None,
    
    /**
     * @brief Encode (compress) files using Huffman algorithm
     * 
     * Applies Huffman compression to single files, multiple files, or
     * directory structures (with -r flag). Generates a .huf archive
     * containing compressed data and metadata headers.
     */
    Encode,
    
    /**
     * @brief Decode (decompress) archive files
     * 
     * Decompresses a .huf archive file, restoring the original files
     * and directory structure to their original names and locations.
     * Uses metadata headers to reconstruct the Huffman tree and file layout.
     */
    Decode,
    
    /**
     * @brief Display archive information
     * 
     * Shows the contents of a .huf archive including file listings,
     * directory structure, and compression statistics (with -v flag).
     * Does not modify any files.
     */
    Info
};