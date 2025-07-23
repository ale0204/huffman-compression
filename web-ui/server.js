const express = require('express');
const multer = require('multer');
const path = require('path');
const fs = require('fs-extra');
const { execSync, spawn } = require('child_process');
const cors = require('cors');

const app = express();
const PORT = process.env.PORT || 50204;
const HOST = '0.0.0.0'; // Bind to all interfaces for Docker

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static(path.join(__dirname, 'public')));
app.use('/downloads', express.static(path.join(__dirname, 'downloads')));

// Create directories
const uploadsDir = path.join(__dirname, 'uploads');
const downloadsDir = path.join(__dirname, 'downloads');
const decompressedDir = path.join(__dirname, 'decompressed');

fs.ensureDirSync(uploadsDir);
fs.ensureDirSync(downloadsDir);
fs.ensureDirSync(decompressedDir);

// Configure multer for file uploads (1GB limit)
const storage = multer.diskStorage({
    destination: function (req, file, cb) {
        cb(null, uploadsDir);
    },
    filename: function (req, file, cb) {
        const timestamp = Date.now();
        cb(null, `${timestamp}-${file.originalname}`);
    }
});

const upload = multer({ 
    storage: storage,
    limits: {
        fileSize: 1024 * 1024 * 1024 // 1GB limit
    },
    fileFilter: function (req, file, cb) {
        // Allow all file types
        cb(null, true);
    }
});

// Helper function to execute Huffman compression utility
function executeHuffman(args, callback) {
    const huffmanExe = path.join(__dirname, '..', 'huff');
    
    try {
        const result = execSync(`"${huffmanExe}" ${args}`, {
            cwd: path.join(__dirname, '..'),
            encoding: 'utf8',
            timeout: 60000 // 60 seconds timeout
        });
        callback(null, result);
    } catch (error) {
        callback(error, null);
    }
}

// Helper function to parse verbose output
function parseVerboseOutput(output) {
    const stats = {};
    const lines = output.split('\n');
    let inFrequencyTable = false;
    const frequencyTable = [];
    
    console.log('=== FULL HUFFMAN OUTPUT START ===');
    console.log(output);
    console.log('=== FULL HUFFMAN OUTPUT END ===');
    
    for (const line of lines) {
        const trimmedLine = line.trim();
        console.log('Parsing line:', JSON.stringify(trimmedLine)); // Debug logging with JSON to see whitespace
        
        if (trimmedLine.includes('Original size:') || trimmedLine.includes('Original total size:')) {
            const match = trimmedLine.match(/(\d+)\s*bytes/);
            stats.originalSize = match ? match[1] : null;
        } else if (trimmedLine.includes('Compressed data size:') || trimmedLine.includes('Compressed size:')) {
            const match = trimmedLine.match(/(\d+)\s*bytes/);
            stats.compressedSize = match ? match[1] : null;
        } else if (trimmedLine.includes('Actual compression ratio:') || trimmedLine.includes('Compression ratio:')) {
            const match = trimmedLine.match(/([\d.]+)%/);
            stats.compressionRatio = match ? match[1] : null;
        } else if (trimmedLine.includes('Shannon information:')) {
            const match = trimmedLine.match(/([\d.]+)\s*bits/);
            stats.shannonInfo = match ? match[1] : null;
        } else if (trimmedLine.includes('Huffman average:')) {
            const match = trimmedLine.match(/([\d.]+)\s*bits/);
            stats.huffmanAverage = match ? match[1] : null;
        } else if (trimmedLine.includes('Efficiency:')) {
            const match = trimmedLine.match(/([\d.]+)%/);
            stats.efficiency = match ? match[1] : null;
        } else if (trimmedLine.includes('Files compressed:') || trimmedLine.includes('Number of files:')) {
            const match = trimmedLine.match(/(\d+)/);
            stats.filesCompressed = match ? match[1] : null;
        } else if (trimmedLine.includes('Frequency table entries:')) {
            const match = trimmedLine.match(/(\d+)/);
            stats.frequencyEntries = match ? match[1] : null;
        } else if (trimmedLine.includes('=== CHARACTER FREQUENCY TABLE ===') || 
                   trimmedLine.includes('CHARACTER FREQUENCY TABLE') ||
                   trimmedLine.includes('FREQUENCY TABLE')) {
            console.log('🔍 Found frequency table start marker:', JSON.stringify(trimmedLine));
            inFrequencyTable = true;
        } else if (trimmedLine.includes('Nr') && trimmedLine.includes('Char') && trimmedLine.includes('Freq') && trimmedLine.includes('Code')) {
            console.log('🔍 Found frequency table header:', JSON.stringify(trimmedLine));
            // Skip table headers but stay in frequency table mode
            continue;
        } else if (trimmedLine.includes('----') || trimmedLine.includes('====')) {
            console.log('🔍 Found separator line:', JSON.stringify(trimmedLine));
            // Skip separator lines
            continue;
        } else if (inFrequencyTable && trimmedLine.length > 0) {
            console.log('🔍 Processing frequency table line:', JSON.stringify(trimmedLine));
            // Parse frequency table row - be more robust with whitespace
            const parts = trimmedLine.split(/\s+/); // Split on any whitespace
            console.log('🔍 Split parts:', parts, 'Length:', parts.length);
            
            if (parts.length >= 4) { // Relaxed from 5 to 4 minimum parts
                const index = parts[0];
                const char = parts[1];
                const freq = parts[2];
                const code = parts[3];
                
                // Parse and validate frequency - ensure it's a valid number
                const parsedFreq = parseInt(freq);
                const parsedIndex = parseInt(index);
                
                console.log('🔍 Attempting to parse:', {
                    index: index,
                    parsedIndex: parsedIndex,
                    char: char,
                    freq: freq,
                    parsedFreq: parsedFreq,
                    code: code
                });
                
                // The bits value might be in parts[4] or later due to code formatting
                let bits = null;
                for (let i = 4; i < parts.length; i++) {
                    if (parts[i] && !isNaN(parseInt(parts[i]))) {
                        bits = parseInt(parts[i]);
                        break;
                    }
                }
                
                // Fallback: calculate bits from code length if not found
                if (!bits && code) {
                    bits = code.length;
                }
                
                // Only add valid entries to the frequency table
                if (!isNaN(parsedIndex) && !isNaN(parsedFreq) && code && code !== 'Code') {
                    const entry = {
                        index: parsedIndex,
                        character: char,
                        frequency: parsedFreq,
                        code: code,
                        bits: bits || (code ? code.length : 0)
                    };
                    console.log('✅ Adding entry to frequency table:', entry);
                    frequencyTable.push(entry);
                } else {
                    console.log('❌ Skipping invalid entry:', {
                        parsedIndex: parsedIndex,
                        parsedFreq: parsedFreq,
                        code: code,
                        isValidIndex: !isNaN(parsedIndex),
                        isValidFreq: !isNaN(parsedFreq),
                        hasCode: !!code,
                        isNotHeader: code !== 'Code'
                    });
                }
            } else if (trimmedLine === '' || parts.length === 0) {
                console.log('🔍 Empty line - ending frequency table');
                // End of frequency table
                inFrequencyTable = false;
            } else {
                console.log('🔍 Line has insufficient parts:', parts.length, '- skipping');
            }
        }
    }
    
    stats.frequencyTable = frequencyTable;
    console.log('📊 Final parsed stats:', stats);
    console.log('📊 Frequency table entries found:', frequencyTable.length);
    if (frequencyTable.length > 0) {
        console.log('📊 First entry:', frequencyTable[0]);
        console.log('📊 Last entry:', frequencyTable[frequencyTable.length - 1]);
    }
    return stats;
}

// Route: Upload and compress file
app.post('/upload', upload.single('file'), (req, res) => {
    if (!req.file) {
        return res.status(400).json({ error: 'No file uploaded' });
    }

    const inputFile = req.file.path;
    const outputFile = path.join(downloadsDir, `${req.file.filename}.huf`);
    
    // Check if uploaded file is already a Huffman archive
    const isArchive = req.file.originalname.endsWith('.huf');
    
    if (isArchive) {
        // It's an archive - decode and list contents
        const tempDecompressDir = path.join(decompressedDir, req.file.filename);
        fs.ensureDirSync(tempDecompressDir);
        
        const args = `-d -v "${inputFile}" -o "${tempDecompressDir}"`;
        
        executeHuffman(args, (error, output) => {
            if (error) {
                console.error('Decompression error:', error);
                return res.status(500).json({ 
                    error: 'Failed to decompress archive',
                    details: error.message 
                });
            }
            
            console.log('Decompression output:'); // Debug logging
            console.log(output); // Debug logging
            
            try {
                // List files in decompressed directory
                const files = fs.readdirSync(tempDecompressDir);
                const fileList = files.map(filename => {
                    const filePath = path.join(tempDecompressDir, filename);
                    const stats = fs.statSync(filePath);
                    return {
                        name: filename,
                        size: stats.size,
                        path: path.relative(__dirname, filePath).replace(/\\/g, '/')
                    };
                });
                
                // Parse statistics from output
                const compressionStats = parseVerboseOutput(output);
                
                res.json({
                    type: 'archive',
                    originalName: req.file.originalname,
                    files: fileList,
                    stats: compressionStats,
                    decompressedPath: path.relative(__dirname, tempDecompressDir).replace(/\\/g, '/'),
                    output: output
                });
            } catch (listError) {
                console.error('Error listing decompressed files:', listError);
                res.status(500).json({ 
                    error: 'Failed to list decompressed files',
                    details: listError.message 
                });
            }
        });
    } else {
        // Regular file - compress it
        const args = `-e -v "${inputFile}" -o "${outputFile}"`;
        
        executeHuffman(args, (error, output) => {
            if (error) {
                console.error('Compression error:', error);
                return res.status(500).json({ 
                    error: 'Compression failed',
                    details: error.message 
                });
            }
            
            try {
                const outputStats = fs.statSync(outputFile);
                const compressionStats = parseVerboseOutput(output);
                
                res.json({
                    type: 'compressed',
                    originalName: req.file.originalname,
                    compressedFile: path.relative(__dirname, outputFile).replace(/\\/g, '/'),
                    compressedSize: outputStats.size,
                    stats: compressionStats,
                    output: output
                });
            } catch (statError) {
                console.error('Error reading compressed file stats:', statError);
                res.status(500).json({ 
                    error: 'Failed to read compressed file information',
                    details: statError.message 
                });
            }
        });
    }
});

// Route: Download file
app.get('/download/:path(*)', (req, res) => {
    const filePath = path.join(__dirname, req.params.path);
    
    if (!fs.existsSync(filePath)) {
        return res.status(404).json({ error: 'File not found' });
    }
    
    const filename = path.basename(filePath);
    res.download(filePath, filename);
});

// Route: Download all files as zip (for decompressed archives)
app.post('/download-all', (req, res) => {
    const { decompressedPath } = req.body;
    
    if (!decompressedPath) {
        return res.status(400).json({ error: 'No path specified' });
    }
    
    const fullPath = path.join(__dirname, decompressedPath);
    
    if (!fs.existsSync(fullPath)) {
        return res.status(404).json({ error: 'Path not found' });
    }
    
    // Create a simple tar-like archive (just for demo - you could use a proper zip library)
    const archiveName = `extracted-${Date.now()}.zip`;
    const archivePath = path.join(downloadsDir, archiveName);
    
    try {
        // For now, we'll just return the path to download individual files
        // In a full implementation, you'd create a proper zip archive
        res.json({
            message: 'Use individual file download links',
            files: fs.readdirSync(fullPath)
        });
    } catch (error) {
        res.status(500).json({ 
            error: 'Failed to create archive',
            details: error.message 
        });
    }
});

// Route: Clean up old files
app.delete('/cleanup', (req, res) => {
    try {
        // Clean files older than 1 hour
        const oneHourAgo = Date.now() - (60 * 60 * 1000);
        
        [uploadsDir, downloadsDir, decompressedDir].forEach(dir => {
            const files = fs.readdirSync(dir);
            files.forEach(file => {
                const filePath = path.join(dir, file);
                const stats = fs.statSync(filePath);
                if (stats.mtime.getTime() < oneHourAgo) {
                    fs.removeSync(filePath);
                }
            });
        });
        
        res.json({ message: 'Cleanup completed' });
    } catch (error) {
        res.status(500).json({ 
            error: 'Cleanup failed',
            details: error.message 
        });
    }
});

// Main route
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Error handling middleware
app.use((error, req, res, next) => {
    if (error instanceof multer.MulterError) {
        if (error.code === 'LIMIT_FILE_SIZE') {
            return res.status(400).json({ 
                error: 'File too large. Maximum size is 1GB.' 
            });
        }
    }
    
    console.error('Server error:', error);
    res.status(500).json({ 
        error: 'Internal server error',
        details: error.message 
    });
});

// Docker-optimized server startup
app.listen(PORT, HOST, () => {
    console.log(`🚀 Huffman Compression Web UI running on http://${HOST}:${PORT}`);
    console.log(`📁 Uploads: ${uploadsDir}`);
    console.log(`📦 Downloads: ${downloadsDir}`);
    console.log(`📂 Decompressed: ${decompressedDir}`);
    console.log(`🔧 Huffman executable: ${path.join(__dirname, '..', 'huff')}`);
    console.log(`🐳 Docker container ready for external connections!`);
    
    // Health check endpoint for Docker
    app.get('/health', (req, res) => {
        res.status(200).json({ 
            status: 'healthy',
            timestamp: new Date().toISOString(),
            uptime: process.uptime()
        });
    });
});

module.exports = app;
