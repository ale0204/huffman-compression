#pragma once
#include <exception>
#include <string>

/**
 * @brief Error codes for different types of Huffman compression utility errors
 * 
 * This enum class defines specific error codes for different categories of
 * errors that can occur during Huffman compression operations. Each error
 * code represents a specific failure scenario and can be used for detailed
 * error handling and user feedback.
 */
enum class HuffmanErrorCode {
    /**
     * @brief Unknown or unspecified error occurred
     * 
     * Default error code used when the specific error type cannot be determined
     * or when creating exceptions without specifying a particular error category.
     */
    Unknown = -1,
    
    /**
     * @brief Command line flag missing required argument
     * 
     * Thrown when a command line option that requires an argument (like -o for output file)
     * is provided without the expected argument value following it.
     */
    MissingArgument = 1,
    
    /**
     * @brief Required output file not specified for encoding operation
     * 
     * Thrown when attempting to encode files but no output file was specified using
     * the -o flag, which is mandatory for compression operations.
     */
    MissingOutputFile = 2,
    
    /**
     * @brief Unrecognized command line option provided
     * 
     * Thrown when the user provides a command line flag or option that is not
     * recognized by the program (e.g., -x or --invalid-option).
     */
    UnknownOption = 3,
    
    /**
     * @brief Invalid operation mode combination or missing operation mode
     * 
     * Thrown when multiple conflicting operation modes are specified (e.g., both -e and -d),
     * when no operation mode is specified, or when invalid combinations of flags are used.
     */
    InvalidMode = 4,
    
    /**
     * @brief File input/output operation failed
     * 
     * Thrown when file operations fail, such as inability to open, read from, or write to
     * files. This includes permission errors, missing files, and disk space issues.
     */
    FileError = 5,
    
    /**
     * @brief Huffman compression or decompression algorithm failed
     * 
     * Thrown when the Huffman encoding or decoding process encounters an error,
     * such as invalid frequency tables, corrupted compressed data, or algorithm failures.
     */
    CompressionError = 6,
    
    /**
     * @brief Invalid or corrupted archive format detected
     * 
     * Thrown when attempting to read an archive file that has an invalid format,
     * corrupted headers, missing metadata, or incompatible version.
     */
    ArchiveFormatError = 7
};

/**
 * @brief Custom exception class for Huffman compression utility errors
 * 
 * This exception class is used throughout the Huffman compression utility
 * to handle various error conditions such as missing command line arguments,
 * invalid file operations, compression/decompression errors, and other
 * application-specific errors. It provides both human-readable error messages
 * and structured error codes for programmatic error handling.
 */
class HuffmanException : public std::exception {
private:
    std::string message;          ///< Human-readable error message
    HuffmanErrorCode errorCode;   ///< Structured error code for programmatic handling

public:
    /**
     * @brief Construct a new Huffman Exception with a message
     * 
     * Creates a new exception with a custom error message and optional error code.
     * The error code defaults to Unknown if not specified.
     * 
     * @param msg The error message describing what went wrong
     * @param code Optional error code (default: HuffmanErrorCode::Unknown)
     */
    explicit HuffmanException(const std::string& msg, HuffmanErrorCode code = HuffmanErrorCode::Unknown);

    /**
     * @brief Construct a new Huffman Exception with a C-style string message
     * 
     * Creates a new exception with a C-style string error message and optional error code.
     * The error code defaults to Unknown if not specified.
     * 
     * @param msg The error message describing what went wrong
     * @param code Optional error code (default: HuffmanErrorCode::Unknown)
     */
    explicit HuffmanException(const char* msg, HuffmanErrorCode code = HuffmanErrorCode::Unknown);

    /**
     * @brief Get the exception message
     * 
     * Returns the human-readable error message that describes what went wrong.
     * This overrides the standard std::exception::what() method.
     * 
     * @return const char* The error message as a null-terminated string
     */
    virtual const char* what() const noexcept override;

    /**
     * @brief Get the error code associated with this exception
     * 
     * Returns the structured error code that can be used for programmatic
     * error handling and categorization of different error types.
     * 
     * @return HuffmanErrorCode The error code enum value
     */
    HuffmanErrorCode getErrorCode() const noexcept;

    // Static factory methods for common error types

    /**
     * @brief Create exception for missing required command line argument
     * 
     * Factory method that creates a HuffmanException for cases where a command line
     * flag that requires an argument (like -o for output file) is provided without
     * the expected argument value following it.
     * 
     * @param flag The flag that requires an argument (e.g., "-o", "--output")
     * @return HuffmanException Configured exception with MissingArgument error code
     */
    static HuffmanException missingArgument(const std::string& flag);

    /**
     * @brief Create exception for missing required output file
     * 
     * Factory method that creates a HuffmanException for encoding operations
     * where no output file was specified using the -o flag, which is mandatory
     * for compression operations.
     * 
     * @return HuffmanException Configured exception with MissingOutputFile error code
     */
    static HuffmanException missingOutputFile();

    /**
     * @brief Create exception for unknown command line option
     * 
     * Factory method that creates a HuffmanException when the user provides
     * a command line flag or option that is not recognized by the program.
     * 
     * @param option The unknown option that was provided (e.g., "-x", "--invalid")
     * @return HuffmanException Configured exception with UnknownOption error code
     */
    static HuffmanException unknownOption(const std::string& option);

    /**
     * @brief Create exception for invalid operation mode combination
     * 
     * Factory method that creates a HuffmanException for cases involving
     * multiple conflicting operation modes, missing operation modes, or
     * invalid combinations of command line flags.
     * 
     * @param message Description of the invalid combination or mode issue
     * @return HuffmanException Configured exception with InvalidMode error code
     */
    static HuffmanException invalidMode(const std::string& message);

    /**
     * @brief Create exception for file operation errors
     * 
     * Factory method that creates a HuffmanException for file I/O failures
     * such as inability to open, read from, or write to files. This includes
     * permission errors, missing files, and disk space issues.
     * 
     * @param filename The file that caused the error
     * @param operation The operation that failed (e.g., "open", "read", "write")
     * @return HuffmanException Configured exception with FileError error code
     */
    static HuffmanException fileError(const std::string& filename, const std::string& operation);

    /**
     * @brief Create exception for compression/decompression errors
     * 
     * Factory method that creates a HuffmanException for errors that occur
     * during the Huffman encoding or decoding process, such as invalid frequency
     * tables, corrupted compressed data, or algorithm failures.
     * 
     * @param message Description of the compression/decompression error
     * @return HuffmanException Configured exception with CompressionError error code
     */
    static HuffmanException compressionError(const std::string& message);

    /**
     * @brief Create exception for archive format errors
     * 
     * Factory method that creates a HuffmanException for cases where an archive
     * file has an invalid format, corrupted headers, missing metadata, or
     * incompatible version information.
     * 
     * @param message Description of the archive format issue
     * @return HuffmanException Configured exception with ArchiveFormatError error code
     */
    static HuffmanException archiveFormatError(const std::string& message);
};
