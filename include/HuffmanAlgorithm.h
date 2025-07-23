#pragma once
#include "HuffmanNode.h"
#include "ArchiveStructures.h"
#include "CommandLineOptions.h"
#include "OperationMode.h"
#include "HuffmanException.h"
#include <string>
#include <map>
#include <vector>
#include <iostream>

/**
 * @brief Core Huffman compression algorithm implementation
 * 
 * This class contains all the main algorithms for Huffman compression
 * including frequency analysis, tree construction, code generation,
 * and text encoding/decoding operations.
 */
class HuffmanAlgorithm
{
public:
    /**
     * @brief Build character frequency table from input text
     * 
     * Analyzes the input text and counts the frequency of each character.
     * This frequency table is used to construct the optimal Huffman tree.
     * 
     * @param text The input text to analyze
     * @return std::map<char, int> Map of characters to their frequencies
     */
    static std::map<char, int> buildFrequencyTable(const std::string& text);
    
    /**
     * @brief Build Huffman tree from character frequencies
     * 
     * Constructs the optimal Huffman tree using a priority queue algorithm.
     * The tree is built bottom-up by repeatedly combining the two nodes
     * with the lowest frequencies.
     * 
     * @param frequencies Map of characters to their frequencies
     * @return HuffmanNode* Pointer to the root of the constructed tree
     */
    static HuffmanNode* buildHuffmanTree(const std::map<char, int>& frequencies);
    
    /**
     * @brief Generate Huffman codes from the tree
     * 
     * Traverses the Huffman tree to generate the optimal binary codes
     * for each character. Left edges represent '0' and right edges '1'.
     * 
     * @param root Pointer to the root of the Huffman tree
     * @param code Current code being built (used for recursion)
     * @param codes Output map to store character-to-code mappings
     */
    static void generateCodes(HuffmanNode* root, const std::string& code, 
                             std::map<char, std::string>& codes);
    
    /**
     * @brief Encode text using Huffman codes
     * 
     * Converts input text to a compressed binary string using the
     * provided Huffman codes for each character.
     * 
     * @param text The text to encode
     * @param codes Map of characters to their Huffman codes
     * @return std::string The encoded binary string
     */
    static std::string encodeText(const std::string& text, 
                                 const std::map<char, std::string>& codes);
    
    /**
     * @brief Decode binary string using Huffman tree
     * 
     * Converts a Huffman-encoded binary string back to the original text
     * by traversing the Huffman tree according to the binary digits.
     * 
     * @param encodedText The binary string to decode
     * @param root Pointer to the root of the Huffman tree
     * @return std::string The decoded original text
     */
    static std::string decodeText(const std::string& encodedText, HuffmanNode* root);
    
    /**
     * @brief Calculate Shannon entropy for frequency distribution
     * 
     * Computes the theoretical minimum average bits per character
     * according to Shannon's information theory. This represents
     * the optimal compression ratio achievable.
     * 
     * @param frequencies Map of characters to their frequencies
     * @param totalChars Total number of characters in the text
     * @return double Shannon entropy in bits per character
     */
    static double calculateShannonEntropy(const std::map<char, int>& frequencies, 
                                         int totalChars);
    
    /**
     * @brief Generate complete compression statistics
     * 
     * Analyzes text and generates comprehensive statistics including
     * character frequencies, Huffman codes, compression ratios,
     * and efficiency metrics.
     * 
     * @param text The input text to analyze
     * @return CompressionStatistics Complete statistical analysis
     */
    static CompressionStatistics generateCompressionStatistics(const std::string& text);
    
    /**
     * @brief Perform complete Huffman compression
     * 
     * High-level function that performs the complete compression process:
     * frequency analysis, tree building, code generation, and text encoding.
     * 
     * @param text The text to compress
     * @param outTree Output parameter for the constructed Huffman tree
     * @param outStats Output parameter for compression statistics
     * @return std::string The compressed binary string
     */
    static std::string compressText(const std::string& text, 
                                   HuffmanNode*& outTree, 
                                   CompressionStatistics& outStats);
    
    /**
     * @brief Perform complete Huffman decompression
     * 
     * High-level function that decompresses a Huffman-encoded binary string
     * back to the original text using the provided Huffman tree.
     * 
     * @param encodedText The compressed binary string
     * @param tree The Huffman tree used for encoding
     * @return std::string The decompressed original text
     */
    static std::string decompressText(const std::string& encodedText, 
                                     HuffmanNode* tree);

    /**
     * @brief Encode files based on command line options
     * 
     * High-level function that handles the complete encoding process including
     * reading input files, performing compression, and writing output.
     * 
     * @param options Command line options containing input files and settings
     * @return bool True if encoding was successful, false otherwise
     */
    static bool encodeFiles(const CommandLineOptions& options);

    /**
     * @brief Decode archive based on command line options
     * 
     * High-level function that handles the complete decoding process including
     * reading archive, decompressing data, and writing output files.
     * 
     * @param options Command line options containing input archive and settings
     * @return bool True if decoding was successful, false otherwise
     */
    static bool decodeArchive(const CommandLineOptions& options);

    /**
     * @brief Display archive information based on command line options
     * 
     * High-level function that reads an archive and displays metadata
     * and statistical information about its contents.
     * 
     * @param options Command line options containing archive file and settings
     * @return bool True if info display was successful, false otherwise
     */
    static bool displayArchiveInfo(const CommandLineOptions& options);
};
