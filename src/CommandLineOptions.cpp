#include "../include/CommandLineOptions.h"
#include <cstdlib>

// Constructor implementation
CommandLineOptions::CommandLineOptions(int argc, char *argv[]) 
{
    try 
    {
        parseCommandLine(argc, argv);
        validateOptions();
    } 
    catch (const HuffmanException& e) 
    {
        std::cerr << "Error parsing command line options: " << e.what() << "\n\n";
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }
}

// Getters implementation
OperationMode CommandLineOptions::getMode() const 
{ 
    return mode; 
}

bool CommandLineOptions::isRecursive() const 
{ 
    return recursive; 
}

bool CommandLineOptions::isVerbose() const 
{ 
    return verbose; 
}

const std::string& CommandLineOptions::getOutputFile() const 
{ 
    return outputFile; 
}

const std::vector<std::string>& CommandLineOptions::getInputFiles() const 
{ 
    return inputFiles; 
}

void CommandLineOptions::printUsage(const char* programName) 
{
    std::cout << "Huffman Compression Utility\n";
    std::cout << "Usage: " << programName << " [OPTIONS] [FILES/DIRECTORY]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -e, --encode     Encode (compress) files using Huffman algorithm\n";
    std::cout << "  -d, --decode     Decode (decompress) archive to original files\n";
    std::cout << "  -i, --info       Display archive contents and information\n";
    std::cout << "  -r, --recursive  Operate recursively on directories (encode only)\n";
    std::cout << "  -v, --verbose    Display detailed information and statistics\n";
    std::cout << "  -o, --output     Specify output archive file (required for encode)\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << programName << " -e file1.txt file2.txt -o archive.huf\n";
    std::cout << "  " << programName << " -e -r mydir -o mydir.huf -v\n";
    std::cout << "  " << programName << " -d archive.huf\n";
    std::cout << "  " << programName << " -i archive.huf -v\n";
}

void CommandLineOptions::parseCommandLine(int argc, char* argv[]) 
{
    // Initialize all flags and mode
    recursive = false;
    verbose = false;
    mode = OperationMode::None;
    
    if (argc < 2) 
    {
        throw HuffmanException::invalidMode("No command line arguments provided");
    }
    
    for (int i = 1; i < argc; i++) 
    {
        std::string arg = argv[i];

        if (arg == "-e" || arg == "--encode") 
        {
            if (mode != OperationMode::None) {
                throw HuffmanException::invalidMode("Multiple operation modes specified");
            }
            mode = OperationMode::Encode;
        }
        else if (arg == "-d" || arg == "--decode") 
        {
            if (mode != OperationMode::None) {
                throw HuffmanException::invalidMode("Multiple operation modes specified");
            }
            mode = OperationMode::Decode;
        }
        else if (arg == "-i" || arg == "--info") 
        {
            if (mode != OperationMode::None) {
                throw HuffmanException::invalidMode("Multiple operation modes specified");
            }
            mode = OperationMode::Info;
        }
        else if (arg == "-r" || arg == "--recursive") 
        {
            if (recursive) {
                throw HuffmanException::invalidMode("Recursive flag (-r) specified multiple times");
            }
            recursive = true;
        }
        else if (arg == "-v" || arg == "--verbose") 
        {
            if (verbose) {
                throw HuffmanException::invalidMode("Verbose flag (-v) specified multiple times");
            }
            verbose = true;
        }
        else if (arg == "-o" || arg == "--output") 
        {
            if (!outputFile.empty()) {
                throw HuffmanException::invalidMode("Output file (-o) specified multiple times");
            }
            if (i + 1 < argc) {
                outputFile = argv[++i];
            } else {
                throw HuffmanException::missingArgument("-o");
            }
        }
        else if (arg[0] != '-') 
        {
            // Input file or directory
            inputFiles.push_back(arg);
        }
        else 
        {
            throw HuffmanException::unknownOption(arg);
        }
    }
}

void CommandLineOptions::validateOptions() 
{
    // Check if operation mode was specified
    if (mode == OperationMode::None) 
    {
        throw HuffmanException::invalidMode("No operation mode specified (use -e, -d, or -i)");
    }
    
    // Check encode-specific requirements
    if (mode == OperationMode::Encode) 
    {
        if (inputFiles.empty()) {
            throw HuffmanException::invalidMode("No input files specified for encoding");
        }
        if (outputFile.empty()) {
            throw HuffmanException::missingOutputFile();
        }
    }
    
    // Check decode/info requirements
    if (mode == OperationMode::Decode || mode == OperationMode::Info) 
    {
        if (inputFiles.size() != 1) {
            throw HuffmanException::invalidMode("Exactly one archive file required for decode/info operations");
        }
    }
    
    // Check recursive flag usage
    if (recursive && mode != OperationMode::Encode) 
    {
        throw HuffmanException::invalidMode("Recursive flag (-r) can only be used with encode (-e)");
    }
}
