#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cstdint>

/**
 * @brief Metadata for individual files within an archive
 * 
 * Contains information about a single file that has been compressed
 * and stored in a Huffman archive, including its original size,
 * compressed size, and location within the archive.
 */
struct FileEntry {
    std::string filename;       ///< Original filename without path
    std::string relativePath;   ///< Relative path from compression root
    size_t originalSize;        ///< Size of file before compression
    size_t compressedSize;      ///< Size of file after compression
    size_t offsetInArchive;     ///< Byte offset where file data starts in archive
    
    /**
     * @brief Default constructor
     * 
     * Initializes all numeric fields to zero and strings to empty.
     */
    FileEntry();
    
    /**
     * @brief Construct a new File Entry with basic information
     * 
     * @param name The original filename
     * @param path The relative path from compression root
     * @param origSize The original file size in bytes
     */
    FileEntry(const std::string& name, const std::string& path, size_t origSize);
};

/**
 * @brief Statistical information about Huffman compression results
 * 
 * Contains detailed statistics about the compression process including
 * character frequencies, generated Huffman codes, and efficiency metrics.
 */
struct CompressionStatistics {
    std::map<char, int> frequencies;        ///< Character frequency table
    std::map<char, std::string> huffmanCodes; ///< Generated Huffman codes for each character
    std::map<char, int> codeLengths;        ///< Length of each Huffman code in bits
    double shannonInfo;                     ///< Shannon information content (theoretical optimum)
    double huffmanAverage;                  ///< Average bits per character using Huffman coding
    double compressionRatio;                ///< Compression ratio as percentage
    size_t totalOriginalSize;               ///< Total size of all files before compression
    size_t totalCompressedSize;             ///< Total size of all files after compression
    double efficiency;                      ///< Huffman efficiency vs Shannon limit
    
    /**
     * @brief Default constructor
     * 
     * Initializes all numeric fields to zero and maps to empty.
     */
    CompressionStatistics();
    
    /**
     * @brief Print detailed compression statistics to stdout
     * 
     * Displays compression metrics including original/compressed sizes,
     * compression ratio, Shannon entropy, Huffman efficiency, and a
     * detailed character frequency table with generated codes.
     */
    void printVerboseStatistics() const;
    
    /**
     * @brief Serialize compression statistics to binary format
     * 
     * Converts the statistics structure to a binary representation
     * suitable for storage in archive files.
     * 
     * @return std::vector<uint8_t> Binary representation of the statistics
     */
    std::vector<uint8_t> serialize() const;
    
    /**
     * @brief Deserialize compression statistics from binary format
     * 
     * Reconstructs a CompressionStatistics object from binary data
     * previously created by serialize().
     * 
     * @param data Binary data containing serialized statistics
     * @return CompressionStatistics Reconstructed statistics object
     */
    static CompressionStatistics deserialize(const std::vector<uint8_t>& data);
};

/**
 * @brief Complete metadata for a Huffman archive
 * 
 * Contains all information needed to describe and reconstruct the contents
 * of a Huffman compressed archive, including file listings and compression
 * statistics.
 */
struct ArchiveMetadata {
    std::vector<FileEntry> files;        ///< List of all files in the archive
    CompressionStatistics stats;         ///< Compression statistics and metrics
    std::string compressionMethod;       ///< Name of compression algorithm used
    std::string timestamp;               ///< Creation timestamp of the archive
    
    /**
     * @brief Default constructor
     * 
     * Initializes compression method to "Huffman" and other fields to defaults.
     */
    ArchiveMetadata();
    
    /**
     * @brief Print archive information to stdout
     * 
     * Displays the list of files in the archive with optional detailed
     * information including file sizes and compression statistics.
     * 
     * @param verbose If true, shows detailed file sizes and compression stats
     */
    void printArchiveInfo(bool verbose) const;
    
    /**
     * @brief Serialize archive metadata to binary format
     * 
     * Converts the complete archive metadata to a binary representation
     * suitable for storage as archive headers.
     * 
     * @return std::vector<uint8_t> Binary representation of the metadata
     */
    std::vector<uint8_t> serialize() const;
    
    /**
     * @brief Deserialize archive metadata from binary format
     * 
     * Reconstructs an ArchiveMetadata object from binary data
     * previously created by serialize().
     * 
     * @param data Binary data containing serialized metadata
     * @return ArchiveMetadata Reconstructed metadata object
     */
    static ArchiveMetadata deserialize(const std::vector<uint8_t>& data);
};
