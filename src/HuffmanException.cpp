#include "../include/HuffmanException.h"
#include <sstream>

// Constructors
HuffmanException::HuffmanException(const std::string& msg, HuffmanErrorCode code) 
    : message(msg), errorCode(code) {
}

HuffmanException::HuffmanException(const char* msg, HuffmanErrorCode code) 
    : message(msg), errorCode(code) {
}

// Override what() method
const char* HuffmanException::what() const noexcept {
    return message.c_str();
}

// Get error code
HuffmanErrorCode HuffmanException::getErrorCode() const noexcept {
    return errorCode;
}

// Static factory methods for common error types

HuffmanException HuffmanException::missingArgument(const std::string& flag) 
{
    std::ostringstream oss;
    oss << "Error: " << flag << " requires an argument";
    return HuffmanException(oss.str(), HuffmanErrorCode::MissingArgument);
}

HuffmanException HuffmanException::missingOutputFile() 
{
    return HuffmanException("Error: Output file (-o) is required for encoding operations", HuffmanErrorCode::MissingOutputFile);
}

HuffmanException HuffmanException::unknownOption(const std::string& option) 
{
    std::ostringstream oss;
    oss << "Error: Unknown option '" << option << "'";
    return HuffmanException(oss.str(), HuffmanErrorCode::UnknownOption);
}

HuffmanException HuffmanException::invalidMode(const std::string& message)
{
    std::ostringstream oss;
    oss << "Error: Invalid operation mode - " << message;
    return HuffmanException(oss.str(), HuffmanErrorCode::InvalidMode);
}

HuffmanException HuffmanException::fileError(const std::string& filename, const std::string& operation) 
{
    std::ostringstream oss;
    oss << "Error: Failed to " << operation << " file '" << filename << "'";
    return HuffmanException(oss.str(), HuffmanErrorCode::FileError);
}

HuffmanException HuffmanException::compressionError(const std::string& message)
{
    std::ostringstream oss;
    oss << "Error: Compression/Decompression failed - " << message;
    return HuffmanException(oss.str(), HuffmanErrorCode::CompressionError);
}

HuffmanException HuffmanException::archiveFormatError(const std::string& message)
{
    std::ostringstream oss;
    oss << "Error: Invalid archive format - " << message;
    return HuffmanException(oss.str(), HuffmanErrorCode::ArchiveFormatError);
}
