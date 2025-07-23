#include "../include/ArchiveStructures.h"

FileEntry::FileEntry() 
    : filename(""), relativePath(""), originalSize(0), compressedSize(0), offsetInArchive(0)
{
}

FileEntry::FileEntry(const std::string& name, const std::string& path, size_t origSize)
    : filename(name), relativePath(path), originalSize(origSize), compressedSize(0), offsetInArchive(0)
{
}

CompressionStatistics::CompressionStatistics()
    : shannonInfo(0.0), huffmanAverage(0.0), compressionRatio(0.0), 
      totalOriginalSize(0), totalCompressedSize(0), efficiency(0.0)
{
}

void CompressionStatistics::printVerboseStatistics() const
{
    std::cout << "\n=== COMPRESSION STATISTICS ===\n";
    std::cout << "Original size: " << totalOriginalSize << " bytes\n";
    std::cout << "Compressed size: " << totalCompressedSize << " bytes\n";
    std::cout << "Compression ratio: " << compressionRatio << "%\n";
    std::cout << "Shannon information: " << shannonInfo << " bits/character\n";
    std::cout << "Huffman average: " << huffmanAverage << " bits/character\n";
    std::cout << "Efficiency: " << efficiency << "%\n\n";
    
    std::cout << "=== CHARACTER FREQUENCY TABLE ===\n";
    std::cout << "Nr\tChar\tFreq\tCode\t\t#bits\n";
    std::cout << "--------------------------------------------\n";
    
    int index = 0;
    for (const auto& pair : frequencies)
    {
        char ch = pair.first;
        int freq = pair.second;
        std::string code = huffmanCodes.at(ch);
        int bits = codeLengths.at(ch);
        
        std::cout << index++ << "\t";
        if (ch == ' ')
        {
            std::cout << "SPC";
        }
        else if (ch == '\n')
        {
            std::cout << "\\n";
        }
        else if (ch == '\t')
        {
            std::cout << "\\t";
        }
        else
        {
            std::cout << "'" << ch << "'";
        }
        std::cout << "\t" << freq << "\t" << code << "\t\t" << bits << "\n";
    }
    std::cout << std::endl;
}

std::vector<uint8_t> CompressionStatistics::serialize() const
{
    // TODO: Implement binary serialization of compression statistics
    std::vector<uint8_t> result;
    // This would serialize the statistics structure to binary format
    return result;
}

CompressionStatistics CompressionStatistics::deserialize(const std::vector<uint8_t>& data)
{
    // TODO: Implement binary deserialization of compression statistics
    (void)data; // Mark parameter as unused to avoid compiler warning
    CompressionStatistics stats;
    // This would deserialize the binary data back to statistics structure
    return stats;
}

ArchiveMetadata::ArchiveMetadata()
    : compressionMethod("Huffman"), timestamp("")
{
}

void ArchiveMetadata::printArchiveInfo(bool verbose) const
{
    std::cout << "Archive contains " << files.size() << " file(s):\n";
    
    for (const auto& file : files)
    {
        std::cout << "  " << file.relativePath;
        if (verbose)
        {
            std::cout << " (" << file.originalSize << " -> " 
                      << file.compressedSize << " bytes)";
        }
        std::cout << "\n";
    }
    
    if (verbose)
    {
        stats.printVerboseStatistics();
    }
}

std::vector<uint8_t> ArchiveMetadata::serialize() const
{
    // TODO: Implement binary serialization of archive metadata
    std::vector<uint8_t> result;
    // This would serialize the metadata structure to binary format
    return result;
}

ArchiveMetadata ArchiveMetadata::deserialize(const std::vector<uint8_t>& data)
{
    // TODO: Implement binary deserialization of archive metadata
    (void)data; // Mark parameter as unused to avoid compiler warning
    ArchiveMetadata metadata;
    // This would deserialize the binary data back to metadata structure
    return metadata;
}
