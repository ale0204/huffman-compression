#pragma once
#include "OperationMode.h"
#include "HuffmanException.h"
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Command line options parser and validator for Huffman compression utility
 * 
 * This class handles parsing command line arguments, validating option combinations,
 * and providing access to parsed options through getter methods. It automatically
 * validates the parsed options and throws appropriate exceptions for invalid input.
 */
class CommandLineOptions {
private:
    // Flags for different operations
    OperationMode mode;           ///< The operation mode (encode, decode, info)
    bool recursive;               ///< Whether to operate recursively on directories
    bool verbose;                 ///< Whether to display verbose output
    std::string outputFile;       ///< Output file path for encoding operations
    std::vector<std::string> inputFiles; ///< List of input files or directories

public:
    /**
     * @brief Construct a new Command Line Options object
     * 
     * Parses the command line arguments and validates the option combinations.
     * If parsing fails or invalid combinations are detected, prints usage and exits.
     * 
     * @param argc Number of command line arguments
     * @param argv Array of command line argument strings
     */
    CommandLineOptions(int argc, char *argv[]);

    /**
     * @brief Get the operation mode
     * @return OperationMode The parsed operation mode (Encode, Decode, or Info)
     */
    OperationMode getMode() const;
    
    /**
     * @brief Check if recursive mode is enabled
     * @return bool True if -r/--recursive flag was specified
     */
    bool isRecursive() const;
    
    /**
     * @brief Check if verbose mode is enabled
     * @return bool True if -v/--verbose flag was specified
     */
    bool isVerbose() const;
    
    /**
     * @brief Get the output file path
     * @return const std::string& The output file path specified with -o/--output
     */
    const std::string& getOutputFile() const;
    
    /**
     * @brief Get the list of input files
     * @return const std::vector<std::string>& Vector of input file/directory paths
     */
    const std::vector<std::string>& getInputFiles() const;

    /**
     * @brief Print usage information to stdout
     * 
     * Displays the program usage, available options, and example commands.
     * 
     * @param programName The name of the program executable
     */
    void printUsage(const char* programName);

private:
    /**
     * @brief Parse command line arguments
     * 
     * Processes all command line arguments and sets appropriate member variables.
     * Throws HuffmanException for invalid arguments or combinations.
     * 
     * @param argc Number of command line arguments
     * @param argv Array of command line argument strings
     * @throws HuffmanException For invalid arguments or combinations
     */
    void parseCommandLine(int argc, char* argv[]);
    
    /**
     * @brief Validate parsed command line options
     * 
     * Checks that the parsed options form a valid combination and that
     * all required options are present for the specified operation mode.
     * 
     * @throws HuffmanException For invalid option combinations
     */
    void validateOptions();
};