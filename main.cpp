#include "include/HuffmanAlgorithm.h"

int main(int argc, char* argv[]) 
{
    try 
    {
        // Parse command line arguments and start the program
        CommandLineOptions options = CommandLineOptions(argc, argv);
        
        if (options.isVerbose()) 
        {
            std::cout << "Huffman Compression Utility - Verbose Mode\n";
            std::cout << "==========================================\n";
        }
        
        bool success = false;
        
        if (options.getMode() == OperationMode::Encode)
        {
            success = HuffmanAlgorithm::encodeFiles(options);
        }
        else if (options.getMode() == OperationMode::Decode)
        {
            success = HuffmanAlgorithm::decodeArchive(options);
        }
        else if (options.getMode() == OperationMode::Info)
        {
            success = HuffmanAlgorithm::displayArchiveInfo(options);
        }
        
        if (options.isVerbose() && success)
        {
            std::cout << "Operation completed successfully.\n";
        }
        
        return success ? 0 : 1;
    }
    catch (const HuffmanException& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }
}