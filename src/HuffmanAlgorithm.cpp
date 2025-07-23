#include "../include/HuffmanAlgorithm.h"
#include <queue>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// Helper function to pack 8 bits into a single byte (adapted from old implementation)
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

// Convert bit string to actual binary data
static std::vector<unsigned char> stringToBinaryData(const std::string& bitString)
{
    std::vector<unsigned char> binaryData;
    std::vector<int> bits;
    
    // Convert string of '0' and '1' to vector of integers
    for (char c : bitString)
    {
        bits.push_back(c - '0');
    }
    
    // Pack bits into bytes
    for (size_t i = 0; i < bits.size(); i += 8)
    {
        binaryData.push_back(packBitsIntoByte(bits, i));
    }
    
    return binaryData;
}

// Convert binary data back to bit string (for decompression)
static std::string binaryDataToString(const std::vector<unsigned char>& binaryData, size_t totalValidBits)
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

std::map<char, int> HuffmanAlgorithm::buildFrequencyTable(const std::string& text)
{
    std::map<char, int> frequencies;
    for (char ch : text)
    {
        frequencies[ch]++;
    }
    return frequencies;
}

HuffmanNode* HuffmanAlgorithm::buildHuffmanTree(const std::map<char, int>& frequencies)
{
    // Create a priority queue (min-heap) of HuffmanNode pointers
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, NodeComparator> pq;
    
    // Create leaf nodes for each character and add to priority queue
    for (const auto& pair : frequencies)
    {
        HuffmanNode* node = new HuffmanNode(pair.first, pair.second);
        pq.push(node);
    }
    
    // Build the tree by combining nodes until only one remains
    while (pq.size() > 1)
    {
        // Get the two nodes with lowest frequency
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();
        
        // Create internal node with combined frequency
        HuffmanNode* internal = new HuffmanNode('\0', left->getFrequency() + right->getFrequency());
        internal->setLeft(left);
        internal->setRight(right);
        
        // Add back to priority queue
        pq.push(internal);
    }
    
    // Return the root (last remaining node)
    return pq.empty() ? nullptr : pq.top();
}

void HuffmanAlgorithm::generateCodes(HuffmanNode* root, const std::string& code, 
                                    std::map<char, std::string>& codes)
{
    if (!root) 
        return;
    
    // If this is a leaf node, store the code
    if (root->isLeaf())
    {
        codes[root->getCharacter()] = code.empty() ? "0" : code;
        return;
    }
    
    // Recursively generate codes for left and right subtrees
    generateCodes(root->getLeft(), code + "0", codes);
    generateCodes(root->getRight(), code + "1", codes);
}

std::string HuffmanAlgorithm::encodeText(const std::string& text, 
                                        const std::map<char, std::string>& codes)
{
    std::string encoded;
    for (char ch : text)
    {
        auto it = codes.find(ch);
        if (it != codes.end())
        {
            encoded += it->second;
        }
    }
    return encoded;
}

std::string HuffmanAlgorithm::decodeText(const std::string& encodedText, HuffmanNode* root)
{
    if (!root) return "";
    
    std::string decoded;
    HuffmanNode* current = root;
    
    for (char bit : encodedText)
    {
        // Move left for '0', right for '1'
        if (bit == '0')
        {
            current = current->getLeft();
        }
        else if (bit == '1')
        {
            current = current->getRight();
        }
        
        // If we reached a leaf, add the character and reset to root
        if (current && current->isLeaf())
        {
            decoded += current->getCharacter();
            current = root;
        }
    }
    
    return decoded;
}

double HuffmanAlgorithm::calculateShannonEntropy(const std::map<char, int>& frequencies, 
                                                 int totalChars)
{
    double entropy = 0.0;
    
    for (const auto& pair : frequencies)
    {
        double probability = static_cast<double>(pair.second) / totalChars;
        if (probability > 0)
        {
            entropy -= probability * std::log2(probability);
        }
    }
    
    return entropy;
}

CompressionStatistics HuffmanAlgorithm::generateCompressionStatistics(const std::string& text)
{
    CompressionStatistics stats;
    
    // Build frequency table
    stats.frequencies = buildFrequencyTable(text);
    
    // Build Huffman tree and generate codes
    HuffmanNode* tree = buildHuffmanTree(stats.frequencies);
    generateCodes(tree, "", stats.huffmanCodes);
    
    // Calculate code lengths
    for (const auto& pair : stats.huffmanCodes)
    {
        stats.codeLengths[pair.first] = pair.second.length();
    }
    
    // Calculate statistics
    stats.totalOriginalSize = text.length();
    
    // Calculate compressed size in bits
    size_t compressedBits = 0;
    for (const auto& pair : stats.frequencies)
    {
        char ch = pair.first;
        int freq = pair.second;
        compressedBits += freq * stats.huffmanCodes[ch].length();
    }
    stats.totalCompressedSize = (compressedBits + 7) / 8; // Convert to bytes (rounded up)
    
    // Calculate compression ratio
    if (stats.totalOriginalSize > 0)
    {
        stats.compressionRatio = (1.0 - static_cast<double>(stats.totalCompressedSize) / stats.totalOriginalSize) * 100.0;
    }
    
    // Calculate Shannon entropy
    stats.shannonInfo = calculateShannonEntropy(stats.frequencies, text.length());
    
    // Calculate Huffman average bits per character
    double totalBits = 0.0;
    for (const auto& pair : stats.frequencies)
    {
        totalBits += pair.second * stats.huffmanCodes[pair.first].length();
    }
    stats.huffmanAverage = totalBits / text.length();
    
    // Calculate efficiency (Huffman vs Shannon)
    if (stats.huffmanAverage > 0)
    {
        stats.efficiency = (stats.shannonInfo / stats.huffmanAverage) * 100.0;
    }
    
    // Clean up tree memory
    // TODO: Implement proper tree cleanup function
    
    return stats;
}

std::string HuffmanAlgorithm::compressText(const std::string& text, 
                                          HuffmanNode*& outTree, 
                                          CompressionStatistics& outStats)
{
    // Generate compression statistics (includes frequency table and codes)
    outStats = generateCompressionStatistics(text);
    
    // Build tree for output
    outTree = buildHuffmanTree(outStats.frequencies);
    
    // Encode the text
    return encodeText(text, outStats.huffmanCodes);
}

std::string HuffmanAlgorithm::decompressText(const std::string& encodedText, 
                                            HuffmanNode* tree)
{
    return decodeText(encodedText, tree);
}

bool HuffmanAlgorithm::encodeFiles(const CommandLineOptions& options)
{
    try {
        if (options.isVerbose()) {
            std::cout << "Encoding files...\n";
        }
        
        // Read input files and store their content with metadata
        std::string allText;
        std::vector<std::pair<std::string, size_t>> fileInfo; // filename, size
        
        for (const std::string& inputFile : options.getInputFiles())
        {
            if (options.isVerbose())
            {
                std::cout << "Reading file: " << inputFile << "\n";
            }
            
            std::ifstream file(inputFile, std::ios::binary);
            if (!file.is_open())
            {
                std::cerr << "Error: Could not open file " << inputFile << "\n";
                return false;
            }
            
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string fileContent = buffer.str();
            
            // Extract just the filename without path
            size_t lastSlash = inputFile.find_last_of("/\\");
            std::string fileName = (lastSlash != std::string::npos) ? 
                                   inputFile.substr(lastSlash + 1) : inputFile;
            
            fileInfo.push_back({fileName, fileContent.length()});
            allText += fileContent;
            file.close();
        }
        
        if (allText.empty())
        {
            std::cerr << "Error: No content to compress\n";
            return false;
        }
        
        // Perform Huffman compression
        HuffmanNode* tree = nullptr;
        CompressionStatistics stats;
        std::string compressed = compressText(allText, tree, stats);
        
        // Write compressed data to output file
        // Format: [num_files][file_info][original_total_size][frequency_table_size][frequency_table][compressed_data]
        std::string outputFile = options.getOutputFile();
        std::ofstream outFile(outputFile, std::ios::binary);
        if (!outFile.is_open())
        {
            std::cerr << "Error: Could not create output file " << outputFile << "\n";
            delete tree;
            return false;
        }
        
        // Write number of files
        size_t numFiles = fileInfo.size();
        outFile.write(reinterpret_cast<const char*>(&numFiles), sizeof(numFiles));
        
        // Write file metadata
        for (const auto& info : fileInfo)
        {
            size_t nameLength = info.first.length();
            outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
            outFile.write(info.first.c_str(), nameLength);
            outFile.write(reinterpret_cast<const char*>(&info.second), sizeof(info.second));
        }
        
        // Write original total size
        size_t originalSize = allText.length();
        outFile.write(reinterpret_cast<const char*>(&originalSize), sizeof(originalSize));
        
        // Write frequency table
        size_t freqTableSize = stats.frequencies.size();
        outFile.write(reinterpret_cast<const char*>(&freqTableSize), sizeof(freqTableSize));
        
        for (const auto& pair : stats.frequencies)
        {
            outFile.write(&pair.first, sizeof(char));
            outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(int));
        }
        
        // Convert bit string to actual binary data and write it
        std::vector<unsigned char> binaryData = stringToBinaryData(compressed);
        
        // Update stats with actual compressed size
        stats.totalCompressedSize = binaryData.size();
        
        // Write compression statistics
        outFile.write(reinterpret_cast<const char*>(&stats.shannonInfo), sizeof(double));
        outFile.write(reinterpret_cast<const char*>(&stats.huffmanAverage), sizeof(double));
        outFile.write(reinterpret_cast<const char*>(&stats.compressionRatio), sizeof(double));
        outFile.write(reinterpret_cast<const char*>(&stats.efficiency), sizeof(double));
        outFile.write(reinterpret_cast<const char*>(&stats.totalOriginalSize), sizeof(size_t));
        outFile.write(reinterpret_cast<const char*>(&stats.totalCompressedSize), sizeof(size_t));
        
        // Write the number of valid bits in the last byte (for proper decompression)
        size_t totalBits = compressed.length();
        unsigned char paddingBits = (8 - (totalBits % 8)) % 8;  // Number of padding bits in last byte
        outFile.write(reinterpret_cast<const char*>(&paddingBits), sizeof(paddingBits));
        
        // Write the actual compressed binary data
        outFile.write(reinterpret_cast<const char*>(binaryData.data()), binaryData.size());
        outFile.close();
        
        if (options.isVerbose())
        {
            std::cout << "Compression completed. Output written to: " << outputFile << "\n";
            std::cout << "Files compressed: " << numFiles << "\n";
            std::cout << "Original size: " << allText.length() << " bytes\n";
            std::cout << "Compressed data size: " << binaryData.size() << " bytes\n";
            std::cout << "Actual compression ratio: " << (1.0 - (double)binaryData.size() / allText.length()) * 100.0 << "%\n";
            stats.printVerboseStatistics();
        }
        
        // Clean up
        delete tree;
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error during encoding: " << e.what() << "\n";
        return false;
    }
}

bool HuffmanAlgorithm::decodeArchive(const CommandLineOptions& options)
{
    try 
    {
        if (options.isVerbose()) 
        {
            std::cout << "Decoding archive: " << options.getInputFiles()[0] << "\n";
        }
        
        // Read compressed file
        std::string inputFile = options.getInputFiles()[0];
        std::ifstream file(inputFile, std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open archive file " << inputFile << "\n";
            return false;
        }
        
        // Read number of files
        size_t numFiles;
        file.read(reinterpret_cast<char*>(&numFiles), sizeof(numFiles));
        
        // Read file metadata
        std::vector<std::pair<std::string, size_t>> fileInfo;
        for (size_t i = 0; i < numFiles; i++)
        {
            size_t nameLength;
            file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
            
            std::string fileName(nameLength, '\0');
            file.read(&fileName[0], nameLength);
            
            size_t fileSize;
            file.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
            
            fileInfo.push_back({fileName, fileSize});
        }
        
        // Read original total size
        size_t originalSize;
        file.read(reinterpret_cast<char*>(&originalSize), sizeof(originalSize));
        
        // Read frequency table
        size_t freqTableSize;
        file.read(reinterpret_cast<char*>(&freqTableSize), sizeof(freqTableSize));
        
        std::map<char, int> frequencies;
        for (size_t i = 0; i < freqTableSize; i++)
        {
            char ch;
            int freq;
            file.read(&ch, sizeof(char));
            file.read(reinterpret_cast<char*>(&freq), sizeof(int));
            frequencies[ch] = freq;
        }
        
        // Read stored compression statistics
        CompressionStatistics storedStats;
        file.read(reinterpret_cast<char*>(&storedStats.shannonInfo), sizeof(double));
        file.read(reinterpret_cast<char*>(&storedStats.huffmanAverage), sizeof(double));
        file.read(reinterpret_cast<char*>(&storedStats.compressionRatio), sizeof(double));
        file.read(reinterpret_cast<char*>(&storedStats.efficiency), sizeof(double));
        file.read(reinterpret_cast<char*>(&storedStats.totalOriginalSize), sizeof(size_t));
        file.read(reinterpret_cast<char*>(&storedStats.totalCompressedSize), sizeof(size_t));
        storedStats.frequencies = frequencies;
        
        // Read padding bits information
        unsigned char paddingBits;
        file.read(reinterpret_cast<char*>(&paddingBits), sizeof(paddingBits));
        
        // Read the remaining binary data
        std::vector<unsigned char> binaryData;
        unsigned char byte;
        while (file.read(reinterpret_cast<char*>(&byte), sizeof(byte)))
        {
            binaryData.push_back(byte);
        }
        file.close();
        
        if (binaryData.empty())
        {
            std::cerr << "Error: No compressed data found\n";
            return false;
        }
        
        // Convert binary data back to bit string
        size_t totalBits = binaryData.size() * 8 - paddingBits;
        std::string compressedData = binaryDataToString(binaryData, totalBits);

        if (options.isVerbose())
        {
            std::cout << "Reconstructing Huffman tree from frequency table...\n";
            std::cout << "Original total size: " << originalSize << " bytes\n";
            std::cout << "Number of files: " << numFiles << "\n";
            std::cout << "Frequency table entries: " << freqTableSize << "\n";
            std::cout << "Compressed data: " << binaryData.size() << " bytes (" << totalBits << " bits)\n";
            
            // Generate Huffman codes for complete statistics display
            HuffmanNode* tempTree = buildHuffmanTree(frequencies);
            if (tempTree) {
                generateCodes(tempTree, "", storedStats.huffmanCodes);
                
                // Calculate code lengths
                for (const auto& pair : storedStats.huffmanCodes) {
                    storedStats.codeLengths[pair.first] = pair.second.length();
                }
                
                // Print the stored statistics
                storedStats.printVerboseStatistics();
                
                // Clean up temporary tree
                delete tempTree;
            }
        }
        
        // Reconstruct Huffman tree
        HuffmanNode* tree = buildHuffmanTree(frequencies);
        if (!tree)
        {
            std::cerr << "Error: Could not reconstruct Huffman tree\n";
            return false;
        }
        
        // Decompress all data
        std::string allDecompressed = decompressText(compressedData, tree);
        
        if (allDecompressed.length() != originalSize)
        {
            std::cerr << "Warning: Decompressed size (" << allDecompressed.length() 
                      << ") doesn't match expected size (" << originalSize << ")\n";
        }
        
        // If output directory is specified, restore files to that directory
        std::string outputDir = options.getOutputFile();
        if (outputDir.empty())
        {
            outputDir = "decompressed"; // Default directory
        }
        
        // Create output directory if it doesn't exist
        // Note: This is a simplified approach. In a full implementation, 
        // you'd use proper cross-platform directory creation
        std::string createDirCmd = "mkdir \"" + outputDir + "\" 2>nul"; // Windows
        system(createDirCmd.c_str());
        
        if (options.isVerbose())
        {
            std::cout << "Decompressing files to directory: " << outputDir << "\n";
        }
        
        // Restore individual files with their original names in the output directory
        size_t currentPos = 0;
        for (size_t i = 0; i < numFiles; i++)
        {
            const std::string& fileName = fileInfo[i].first;
            size_t fileSize = fileInfo[i].second;
            
            if (currentPos + fileSize > allDecompressed.length())
            {
                std::cerr << "Error: Not enough decompressed data for file " << fileName << "\n";
                delete tree;
                return false;
            }
            
            std::string fileContent = allDecompressed.substr(currentPos, fileSize);
            currentPos += fileSize;
            
            // Create full path in output directory
            std::string fullPath = outputDir + "/" + fileName;
            
            std::ofstream outFile(fullPath, std::ios::binary);
            if (!outFile.is_open())
            {
                std::cerr << "Error: Could not create output file " << fullPath << "\n";
                std::cerr << "Make sure the directory '" << outputDir << "' exists and is writable.\n";
                delete tree;
                return false;
            }
            
            outFile << fileContent;
            outFile.close();
            
            if (options.isVerbose())
            {
                std::cout << "Restored file: " << fullPath << " (" << fileSize << " bytes)\n";
            }
        }
        
        if (options.isVerbose())
        {
            std::cout << "Decoding completed successfully!\n";
            std::cout << "Size verification: " << allDecompressed.length() << " bytes\n";
        }
        
        // Clean up
        delete tree;
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error during decoding: " << e.what() << "\n";
        return false;
    }
}

bool HuffmanAlgorithm::displayArchiveInfo(const CommandLineOptions& options)
{
    try 
    {
        if (options.isVerbose()) 
        {
            std::cout << "Archive information for: " << options.getInputFiles()[0] << "\n";
        }
        
        // Read archive file to get basic information
        std::string inputFile = options.getInputFiles()[0];
        std::ifstream file(inputFile, std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open archive file " << inputFile << "\n";
            return false;
        }
        
        // Get file size
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.close();
        
        std::cout << "Archive file: " << inputFile << "\n";
        std::cout << "Archive size: " << fileSize << " bytes\n";
        
        if (options.isVerbose())
        {
            std::cout << "Note: Complete archive metadata reading would require\n";
            std::cout << "implementation of archive format with stored metadata.\n";
        }
        
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error while reading archive info: " << e.what() << "\n";
        return false;
    }
}
