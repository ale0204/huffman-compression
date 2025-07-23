# Huffman Compression Web Interface

A modern, interactive web interface for the Huffman compression utility, providing an intuitive drag-and-drop experience for file compression and decompression with advanced statistical analysis and visualization.

## Features

### 🎯 **Core Functionality**
- **Drag & Drop Interface**: Easy file upload with visual feedback and hover effects
- **1GB File Limit**: Handles large files efficiently with size validation
- **Dual Operation Modes**: 
  - Compress regular files to `.huf` archives
  - Extract and list contents of `.huf` archives
- **Real-time Processing**: Live progress indicators with animated feedback
- **Individual File Downloads**: Download extracted files one by one with single-click

### 📊 **Advanced Statistics Display**
- **Compression Metrics**: Real-time calculation and display of:
  - Original vs compressed file sizes
  - Compression ratio percentage
  - Shannon information content (theoretical optimum)
  - Huffman average bits per character
  - Algorithm efficiency percentage (Huffman vs Shannon limit)
- **Visual Efficiency Indicators**: Color-coded efficiency ratings
  - 🟢 Excellent (95%+): Green highlighting
  - 🟡 Good (90-95%): Yellow highlighting  
  - 🔴 Poor (<90%): Red highlighting
- **Technical Details**: Educational explanations of binary bit packing and algorithm efficiency

### 📈 **Interactive Frequency Table**
- **Complete Character Analysis**: Shows every character used in compression with:
  - Sequential index number (0, 1, 2, ...)
  - Character representation with special character handling
  - Frequency count (how many times each character appears)
  - Generated Huffman code (binary representation)
  - Code length in bits
- **Smart Character Display**: 
  - `SPACE` for space characters
  - `NEWLINE` for line breaks (\n)
  - `CARRIAGE RETURN` for carriage returns (\r)
  - `TAB` for tab characters
  - `NULL` for null characters (\0)
  - `NON-PRINTABLE` for control characters
  - Quoted display for regular printable characters
- **Multi-Column Sorting**: Click any column header to sort by:
  - **Index**: Default order (0-255 as assigned by algorithm)
  - **Character**: Alphabetical sorting with special characters prioritized
  - **Frequency**: Numerical sorting (find most/least common characters)
  - **Huffman Code**: Sort by code length, then lexicographically
  - **Bits**: Numerical sorting ⭐ (analyze compression efficiency)
- **Visual Sort Indicators**: 
  - ↕️ Sortable columns
  - ↑ Ascending sort active
  - ↓ Descending sort active
- **Responsive Design**: Scrollable table with sticky headers for large datasets

### 🎨 **User Interface Design**
- **Modern Gradient Design**: Inspired by ezyZip with enhanced aesthetics
- **Responsive Layout**: Works seamlessly on desktop, tablet, and mobile devices
- **Smooth Animations**: 
  - Fade-in effects for content sections
  - Scale animations for interactive elements
  - Progress bar animations during processing
  - Hover effects on buttons and interactive elements
- **Intuitive File Selection**: 
  - Large drag-and-drop area with visual feedback
  - Traditional file browser button as fallback
  - Clear file information display with size formatting
- **Professional Color Scheme**:
  - Primary: Modern blue gradients (#3498db, #2980b9)
  - Success: Green indicators for completed operations
  - Warning: Amber highlights for efficiency ratings
  - Error: Red indicators for poor performance/errors

### 🔧 **Technical Features**
- **Automatic File Type Detection**: Recognizes `.huf` archives vs regular files
- **Real-time Validation**: File size checking and error handling
- **Progressive Enhancement**: Works with JavaScript disabled (basic functionality)
- **Memory Efficient**: Streams large files without loading entirely into memory
- **Cross-browser Compatible**: Supports modern browsers with graceful degradation
- **Security Features**:
  - Local processing only (no data sent to external servers)
  - Automatic cleanup of temporary files
  - File size limits to prevent abuse

## Requirements

- Node.js 14 or higher
- The compiled `huff.exe` in the parent directory

## Installation

1. Navigate to the web-ui directory:
```bash
cd web-ui
```

2. Install dependencies:
```bash
npm install
```

3. Start the server:
```bash
npm start
```

4. Open your browser and go to:
```
http://localhost:50204
```

## Development

For development with auto-reload:
```bash
npm run dev
```

## API Endpoints

### POST /upload
Upload a file for compression or decompression
- **Body**: FormData with file (max 1GB)
- **Response**: JSON with results and detailed statistics
- **Features**:
  - Automatic file type detection (`.huf` vs regular files)
  - Verbose statistical output parsing
  - Frequency table extraction and formatting
  - Real-time compression metrics calculation

### GET /download/:path
Download a specific file from the server
- **Parameters**: File path (URL-encoded)
- **Response**: File download with appropriate MIME type
- **Security**: Path validation to prevent directory traversal

### DELETE /cleanup
Clean up temporary files older than 1 hour
- **Response**: JSON confirmation of cleanup operation
- **Automation**: Prevents disk space accumulation from temporary files

## File Structure

```
web-ui/
├── package.json          # Node.js dependencies and scripts
├── server.js            # Express server with advanced parsing
├── public/              # Static web assets
│   ├── index.html      # Main HTML with sortable tables
│   ├── style.css       # Complete CSS with animations
│   └── script.js       # Interactive JavaScript functionality
├── uploads/            # Temporary uploaded files (auto-cleanup)
├── downloads/          # Generated compressed files (auto-cleanup)
└── decompressed/       # Extracted archive contents (auto-cleanup)
```

## UI Components Breakdown

### 🎨 **Main Interface Elements**
- **Header**: Course branding with gradient design
- **Upload Section**: Large drag-and-drop area with file browser fallback
- **Progress Section**: Animated progress bar with status messages
- **Results Section**: Tabbed display for compression vs archive results
- **Statistics Grid**: Color-coded metrics with hover effects
- **Frequency Table**: Sortable data table with responsive design
- **Footer**: Educational context and course information

### 📱 **Responsive Design Features**
- **Mobile-First**: Optimized for touch interfaces
- **Breakpoint Handling**: Adapts to screen sizes from 320px to 4K
- **Touch Gestures**: Supports drag-and-drop on touch devices
- **Performance**: Efficient rendering for large frequency tables

### 🎭 **Animation System**
- **CSS Transitions**: Smooth hover effects (0.3s ease)
- **Keyframe Animations**: Fade-in, scale-in, and pulse effects
- **JavaScript Animations**: Progress bar and data loading
- **Performance Optimized**: GPU-accelerated transforms

## Usage

### 📦 **File Compression Workflow**:
1. **Upload File**: 
   - Drag any file onto the upload area, OR
   - Click "Select File" to browse your computer
   - File is validated (max 1GB size limit)
2. **Process**: 
   - Click "Compress" button
   - Watch real-time progress indicator
   - Algorithm generates frequency table and Huffman tree
3. **Results**: 
   - Download the generated `.huf` archive file
   - View detailed compression statistics:
     - Original size vs compressed size
     - Compression ratio achieved
     - Shannon entropy vs Huffman efficiency
   - Explore the interactive frequency table:
     - See exactly which characters were found
     - View the Huffman codes assigned to each character
     - Sort by frequency to understand text patterns
     - Sort by bits to analyze compression efficiency

### 📂 **Archive Decompression Workflow**:
1. **Upload Archive**: 
   - Drag a `.huf` file onto the upload area
   - System automatically detects it's an archive
2. **Extract**: 
   - Click "Extract" button  
   - Archive is decompressed using stored frequency table
   - Files are restored to their original content
3. **Download Results**:
   - View list of all extracted files with sizes
   - Download files individually with one click
   - Review original compression statistics:
     - See the efficiency achieved during original compression
     - Examine the frequency table that was used
     - Understand the character distribution of the original data

### 📊 **Understanding the Frequency Table**:
The frequency table is an educational goldmine for understanding Huffman compression:

- **Frequency Analysis**: Sort by "Frequency" column to see:
  - Most common characters (should have shortest codes)
  - Rare characters (should have longest codes)
  - Character distribution patterns in your text

- **Efficiency Analysis**: Sort by "Bits" column to see:
  - ⬆️ **Ascending**: Characters with shortest codes (most efficient)
  - ⬇️ **Descending**: Characters with longest codes (least efficient)
  - Verify that frequent characters get short codes

- **Code Analysis**: Sort by "Huffman Code" to see:
  - How the binary tree structure affects code assignment
  - Prefix-free property (no code is prefix of another)
  - Variable-length encoding in action

### 🎓 **Educational Value**:
- **Information Theory**: See Shannon entropy vs actual compression
- **Algorithm Visualization**: Watch Huffman codes being assigned optimally
- **Efficiency Metrics**: Understand why some files compress better than others
- **Binary Representation**: See exactly how text becomes binary codes

## Security Notes

- **Local Processing**: All files are processed locally on the server - no data sent to external services
- **Automatic Cleanup**: Temporary files are automatically removed after 1 hour to prevent disk filling
- **File Size Limits**: 1GB maximum upload size prevents resource abuse and memory issues
- **Path Validation**: Download endpoints validate file paths to prevent directory traversal attacks
- **No Permanent Storage**: User files are never permanently stored - only temporary processing
- **MIME Type Validation**: Proper content-type headers prevent malicious file execution
- **Input Sanitization**: All user inputs are validated and sanitized before processing

## Browser Compatibility

### ✅ **Fully Supported**
- **Chrome 60+**: All features including drag-and-drop, animations, and table sorting
- **Firefox 55+**: Complete functionality with optimal performance
- **Safari 12+**: Full feature support on macOS and iOS
- **Edge 79+**: Modern Chromium-based Edge with all capabilities

### ⚠️ **Limited Support**
- **Internet Explorer**: Not recommended - lacks modern JavaScript features
- **Older Mobile Browsers**: Basic functionality only, some animations may not work

### 🔧 **Required Browser Features**
- **JavaScript ES6+**: For modern syntax and functionality
- **CSS Grid/Flexbox**: For responsive layout
- **Fetch API**: For asynchronous file operations
- **File API**: For drag-and-drop file handling
- **CSS Animations**: For smooth user experience

## Performance Considerations

### 📈 **Optimization Features**
- **Lazy Loading**: Large frequency tables load progressively
- **Virtual Scrolling**: Efficient rendering for tables with 1000+ rows
- **Debounced Sorting**: Prevents excessive re-renders during rapid clicks
- **Memory Management**: Automatic cleanup prevents memory leaks
- **Chunked Processing**: Large files processed in manageable chunks

### 💾 **Resource Usage**
- **Client Memory**: ~10-50MB depending on file size and table complexity
- **Server Memory**: ~100-500MB per concurrent user during processing
- **Network**: Minimal - only file uploads/downloads, no streaming
- **Storage**: Temporary only - automatic cleanup after 1 hour

## Course Information

**Calculatoare Numerice (Numerical Computing)**  
Semester 2, 2024-2025

### 📚 **Educational Objectives**
This comprehensive web interface demonstrates practical application of:

#### **Information Theory Concepts**
- **Shannon Entropy**: Calculate theoretical compression limits
- **Huffman Coding**: Optimal prefix-free variable-length encoding
- **Compression Efficiency**: Compare actual vs theoretical performance
- **Character Frequency Analysis**: Statistical text analysis techniques

#### **Advanced Computer Science Topics**
- **Binary Bit Packing**: Efficient storage using bitwise operations
- **Tree Data Structures**: Huffman tree construction and traversal
- **Greedy Algorithms**: Optimal code assignment strategy
- **File Format Design**: Custom archive format with metadata storage

#### **Modern Web Development**
- **Node.js Backend**: Server-side JavaScript with Express framework
- **Responsive Frontend**: HTML5, CSS3, and modern JavaScript
- **Real-time UI**: Dynamic content updates and interactive visualizations
- **File Processing**: Binary data handling and streaming

#### **Software Engineering Practices**
- **Modular Architecture**: Separation of concerns across components
- **Error Handling**: Robust exception management and user feedback
- **Performance Optimization**: Efficient algorithms and resource management
- **User Experience**: Intuitive interface design and accessibility

### 🎯 **Learning Outcomes**
Upon using this interface, students will understand:

1. **How Huffman Compression Works**: 
   - See actual frequency tables from real files
   - Observe how frequent characters get shorter codes
   - Understand the trade-off between code length and frequency

2. **Information Theory in Practice**:
   - Compare Shannon entropy with actual compression achieved
   - Understand why some files compress better than others
   - See the relationship between data patterns and compression efficiency

3. **Algorithm Efficiency**:
   - Measure actual performance vs theoretical limits
   - Analyze the impact of character distribution on compression
   - Understand when Huffman coding is most effective

4. **Binary Data Representation**:
   - See how text becomes binary codes
   - Understand bit-level file storage
   - Learn about binary file formats and metadata

### 🛠️ **Technical Skills Developed**
- Binary file manipulation and bit packing algorithms
- Statistical analysis of text and data patterns
- Web-based data visualization and interactive tables
- Client-server architecture for file processing applications
- Modern JavaScript development with async/await patterns
