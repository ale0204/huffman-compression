// Global variables
let selectedFile = null;
let isProcessing = false;

// DOM elements
const uploadBox = document.getElementById('uploadBox');
const fileInput = document.getElementById('fileInput');
const fileInfo = document.getElementById('fileInfo');
const fileName = document.getElementById('fileName');
const fileSize = document.getElementById('fileSize');
const processBtn = document.getElementById('processBtn');
const processBtnText = document.getElementById('processBtnText');
const progressSection = document.getElementById('progressSection');
const progressText = document.getElementById('progressText');
const progressFill = document.getElementById('progressFill');
const resultsSection = document.getElementById('resultsSection');
const compressionResult = document.getElementById('compressionResult');
const archiveResult = document.getElementById('archiveResult');

// Initialize event listeners
document.addEventListener('DOMContentLoaded', function() {
    setupEventListeners();
});

function setupEventListeners() {
    // File input change
    fileInput.addEventListener('change', handleFileSelect);
    
    // Add focus/blur events to track file dialog
    fileInput.addEventListener('focus', () => {
        console.log('File input focused - dialog opening');
    });
    
    fileInput.addEventListener('blur', () => {
        console.log('File input blurred - dialog closed');
    });
    
    // Drag and drop
    uploadBox.addEventListener('dragover', handleDragOver);
    uploadBox.addEventListener('dragleave', handleDragLeave);
    uploadBox.addEventListener('drop', handleDrop);
    
    // Add click handler to upload area as backup (but not on the label)
    uploadBox.addEventListener('click', (e) => {
        // Only trigger if clicking on the upload area but not the label/button
        if (!selectedFile && !e.target.closest('.select-btn')) {
            console.log('Upload area clicked, triggering file selection');
            fileInput.click();
        }
    });
}

function handleDragOver(e) {
    e.preventDefault();
    uploadBox.classList.add('dragover');
}

function handleDragLeave(e) {
    e.preventDefault();
    uploadBox.classList.remove('dragover');
}

function handleDrop(e) {
    e.preventDefault();
    uploadBox.classList.remove('dragover');
    
    const files = e.dataTransfer.files;
    if (files.length > 0) {
        handleFileSelect({ target: { files: files } });
    }
}

function handleFileSelect(e) {
    console.log('File select triggered');
    console.log('Event target:', e.target);
    console.log('Files:', e.target.files);
    console.log('Files length:', e.target.files ? e.target.files.length : 'undefined');
    
    const file = e.target.files[0];
    if (!file) {
        console.log('No file selected - files array is empty or undefined');
        return;
    }
    
    console.log('File selected:', file.name, file.size);
    
    // Check file size (1GB limit)
    const maxSize = 1024 * 1024 * 1024; // 1GB
    if (file.size > maxSize) {
        alert('File size exceeds 1GB limit. Please select a smaller file.');
        // Clear the input
        fileInput.value = '';
        return;
    }
    
    selectedFile = file;
    displayFileInfo(file);
}

function displayFileInfo(file) {
    fileName.textContent = file.name;
    fileSize.textContent = formatFileSize(file.size);
    
    // Determine if it's an archive
    const isArchive = file.name.toLowerCase().endsWith('.huf');
    
    if (isArchive) {
        processBtnText.textContent = 'Extract';
        processBtn.innerHTML = '<i class="fas fa-extract"></i> Extract';
    } else {
        processBtnText.textContent = 'Compress';
        processBtn.innerHTML = '<i class="fas fa-compress"></i> Compress';
    }
    
    // Show file info, hide upload box
    uploadBox.style.display = 'none';
    fileInfo.style.display = 'block';
    fileInfo.classList.add('fade-in');
}

function clearFile() {
    console.log('Clearing file');
    selectedFile = null;
    fileInput.value = '';
    
    // Reset UI
    uploadBox.style.display = 'block';
    fileInfo.style.display = 'none';
    progressSection.style.display = 'none';
    resultsSection.style.display = 'none';
    
    uploadBox.classList.add('scale-in');
}

async function processFile() {
    if (!selectedFile || isProcessing) return;
    
    isProcessing = true;
    processBtn.disabled = true;
    
    // Show progress
    showProgress('Processing file...');
    
    try {
        const formData = new FormData();
        formData.append('file', selectedFile);
        
        // Simulate progress
        animateProgress();
        
        const response = await fetch('/upload', {
            method: 'POST',
            body: formData
        });
        
        if (!response.ok) {
            const error = await response.json();
            throw new Error(error.error || 'Upload failed');
        }
        
        const result = await response.json();
        displayResults(result);
        
    } catch (error) {
        console.error('Processing error:', error);
        alert(`Error: ${error.message}`);
        hideProgress();
    } finally {
        isProcessing = false;
        processBtn.disabled = false;
    }
}

function showProgress(message) {
    progressText.textContent = message;
    progressSection.style.display = 'block';
    progressSection.classList.add('fade-in');
    resultsSection.style.display = 'none';
}

function hideProgress() {
    progressSection.style.display = 'none';
}

function animateProgress() {
    let progress = 0;
    const interval = setInterval(() => {
        progress += Math.random() * 15;
        if (progress > 90) progress = 90;
        
        progressFill.style.width = progress + '%';
        
        if (progress >= 90) {
            clearInterval(interval);
        }
    }, 200);
    
    // Complete progress when done
    setTimeout(() => {
        progressFill.style.width = '100%';
        setTimeout(() => {
            hideProgress();
        }, 500);
    }, 2000);
}

function displayResults(result) {
    resultsSection.style.display = 'block';
    resultsSection.classList.add('fade-in');
    
    if (result.type === 'compressed') {
        displayCompressionResults(result);
    } else if (result.type === 'archive') {
        displayArchiveResults(result);
    }
}

function displayCompressionResults(result) {
    compressionResult.style.display = 'block';
    archiveResult.style.display = 'none';
    
    // Setup download button
    const downloadBtn = document.getElementById('downloadCompressed');
    downloadBtn.onclick = () => downloadFile(result.compressedFile, result.originalName + '.huf');
    
    // Display statistics
    displayCompressionStats(result.stats);
}

function displayArchiveResults(result) {
    archiveResult.style.display = 'block';
    compressionResult.style.display = 'none';
    
    // Display file list
    displayFileList(result.files, result.decompressedPath);
    
    // Setup download all button
    const downloadAllBtn = document.getElementById('downloadAllBtn');
    downloadAllBtn.onclick = () => downloadAllFiles(result.decompressedPath);
    
    // Display statistics
    displayArchiveStats(result.stats);
}

function displayFileList(files, basePath) {
    const fileList = document.getElementById('fileList');
    fileList.innerHTML = '';
    
    files.forEach(file => {
        const fileItem = document.createElement('div');
        fileItem.className = 'file-item';
        
        fileItem.innerHTML = `
            <i class="fas fa-file-alt"></i>
            <div class="file-item-info">
                <div class="file-item-name">${file.name}</div>
                <div class="file-item-size">${formatFileSize(file.size)}</div>
            </div>
            <button class="file-download-btn" onclick="downloadFile('${file.path}', '${file.name}')">
                <i class="fas fa-download"></i>
            </button>
        `;
        
        fileList.appendChild(fileItem);
    });
}

function displayCompressionStats(stats) {
    const statsSection = document.getElementById('compressionStats');
    
    const compressionRatio = parseFloat(stats.compressionRatio) || 0;
    const efficiency = parseFloat(stats.efficiency) || 0;
    
    let efficiencyClass = 'efficiency-excellent';
    if (efficiency < 95) efficiencyClass = 'efficiency-good';
    if (efficiency < 90) efficiencyClass = 'efficiency-poor';
    
    statsSection.innerHTML = `
        <h4><i class="fas fa-chart-bar"></i> Compression Statistics</h4>
        <div class="stats-grid">
            <div class="stat-item">
                <span class="stat-value">${stats.originalSize || 'N/A'}</span>
                <span class="stat-label">Original Size (bytes)</span>
            </div>
            <div class="stat-item">
                <span class="stat-value">${stats.compressedSize || 'N/A'}</span>
                <span class="stat-label">Compressed Size (bytes)</span>
            </div>
            <div class="stat-item">
                <span class="stat-value">${compressionRatio.toFixed(1)}%</span>
                <span class="stat-label">Compression Ratio</span>
            </div>
            <div class="stat-item">
                <span class="stat-value ${efficiencyClass}">${efficiency.toFixed(1)}%</span>
                <span class="stat-label">Huffman Efficiency</span>
            </div>
            <div class="stat-item">
                <span class="stat-value">${stats.shannonInfo || 'N/A'}</span>
                <span class="stat-label">Shannon Info (bits/char)</span>
            </div>
            <div class="stat-item">
                <span class="stat-value">${stats.huffmanAverage || 'N/A'}</span>
                <span class="stat-label">Huffman Avg (bits/char)</span>
            </div>
        </div>
        <div class="mt-20">
            <h5><i class="fas fa-info-circle"></i> Technical Details</h5>
            <p>This compression used advanced binary bit packing to achieve optimal storage density. 
            Each byte stores exactly 8 Huffman-encoded bits, minimizing wasted space and maximizing compression efficiency.</p>
        </div>
    `;
    
    // Show the frequency table if available
    if (stats.frequencyTable && stats.frequencyTable.length > 0) {
        displayFrequencyTable(stats, 'compression');
    }
}

function displayArchiveStats(stats) {
    const statsSection = document.getElementById('archiveStats');
    
    console.log('Archive stats received:', stats); // Debug logging
    
    const compressionRatio = parseFloat(stats.compressionRatio) || 0;
    const efficiency = parseFloat(stats.efficiency) || 0;
    const originalSize = stats.originalSize || 'N/A';
    const filesExtracted = stats.filesCompressed || stats.frequencyEntries || '1';
    
    let efficiencyClass = 'efficiency-excellent';
    if (efficiency < 95 && efficiency > 0) efficiencyClass = 'efficiency-good';
    if (efficiency < 90 && efficiency > 0) efficiencyClass = 'efficiency-poor';
    
    // For archives, we want to show the decompression information
    statsSection.innerHTML = `
        <h4><i class="fas fa-archive"></i> Archive Statistics</h4>
        <div class="stats-grid">
            <div class="stat-item">
                <span class="stat-value">${filesExtracted}</span>
                <span class="stat-label">Files Extracted</span>
            </div>
            <div class="stat-item">
                <span class="stat-value">${originalSize}</span>
                <span class="stat-label">Total Size (bytes)</span>
            </div>
            <div class="stat-item">
                <span class="stat-value">${compressionRatio > 0 ? compressionRatio.toFixed(1) + '%' : 'N/A'}</span>
                <span class="stat-label">Original Compression</span>
            </div>
            <div class="stat-item">
                <span class="stat-value ${efficiencyClass}">${efficiency > 0 ? efficiency.toFixed(1) + '%' : 'N/A'}</span>
                <span class="stat-label">Huffman Efficiency</span>
            </div>
        </div>
        <div class="mt-20">
            <h5><i class="fas fa-check-circle"></i> Extraction Complete</h5>
            <p>All files have been successfully extracted from the Huffman archive. 
            ${efficiency > 0 ? 'The original compression achieved excellent efficiency using binary bit packing technology.' : 'Archive successfully decompressed using advanced Huffman algorithm.'}</p>
        </div>
    `;
    
    // Show the frequency table if available
    if (stats.frequencyTable && stats.frequencyTable.length > 0) {
        displayFrequencyTable(stats, 'archive');
    }
}

async function downloadFile(filePath, filename) {
    try {
        const response = await fetch(`/download/${filePath}`);
        
        if (!response.ok) {
            throw new Error('Download failed');
        }
        
        const blob = await response.blob();
        const url = window.URL.createObjectURL(blob);
        
        const a = document.createElement('a');
        a.href = url;
        a.download = filename;
        document.body.appendChild(a);
        a.click();
        
        window.URL.revokeObjectURL(url);
        document.body.removeChild(a);
        
    } catch (error) {
        console.error('Download error:', error);
        alert(`Download failed: ${error.message}`);
    }
}

async function downloadAllFiles(decompressedPath) {
    // For now, show a message about individual downloads
    // In a full implementation, you could create a zip archive server-side
    alert('Please use the individual download buttons for each file. A "Download All as ZIP" feature can be implemented if needed.');
}

function formatFileSize(bytes) {
    if (bytes === 0) return '0 Bytes';
    
    const k = 1024;
    const sizes = ['Bytes', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    
    return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
}

// Utility function to add animations
function addAnimation(element, animationClass) {
    element.classList.add(animationClass);
    element.addEventListener('animationend', () => {
        element.classList.remove(animationClass);
    }, { once: true });
}

// Clean up function for development
window.clearAll = function() {
    fetch('/cleanup', { method: 'DELETE' })
        .then(response => response.json())
        .then(result => {
            console.log('Cleanup result:', result);
            clearFile();
        })
        .catch(error => console.error('Cleanup error:', error));
};

// Global function to trigger file selection
window.selectFile = function() {
    console.log('Select file button clicked');
    console.log('File input element:', document.getElementById('fileInput'));
    console.log('Selected file before click:', selectedFile);
    
    // Clear any previous selection to ensure change event fires
    fileInput.value = '';
    
    // Trigger file selection
    fileInput.click();
    
    console.log('File input clicked');
};

function displayFrequencyTable(stats, tableType) {
    // tableType can be 'compression' or 'archive'
    const frequencyTableSection = document.getElementById(`${tableType}FrequencyTable`);
    const frequencyTableBody = document.getElementById(`${tableType}FrequencyTableBody`);
    
    if (!stats.frequencyTable || stats.frequencyTable.length === 0) {
        frequencyTableSection.style.display = 'none';
        return;
    }
    
    // Store original data for sorting
    let tableData = [...stats.frequencyTable];
    let currentSort = { column: 'index', direction: 'asc' };
    
    function renderTable(data) {
        // Clear existing table content
        frequencyTableBody.innerHTML = '';
        
        // Populate frequency table
        data.forEach(entry => {
            const row = document.createElement('tr');
            
            // Character display with special handling
            let charDisplay = entry.character;
            let charClass = 'char-display';
            
            switch (entry.character) {
                case 'SPC':
                    charDisplay = 'SPACE';
                    charClass += ' char-special';
                    break;
                case '\\n':
                    charDisplay = 'NEWLINE';
                    charClass += ' char-special';
                    break;
                case '\\r':
                    charDisplay = 'CARRIAGE RETURN';
                    charClass += ' char-special';
                    break;
                case '\\t':
                    charDisplay = 'TAB';
                    charClass += ' char-special';
                    break;
                case '\\0':
                    charDisplay = 'NULL';
                    charClass += ' char-special';
                    break;
                default:
                    if (entry.character.startsWith('[') && entry.character.endsWith(']')) {
                        // Handle non-printable characters like [13], [10], etc.
                        charDisplay = 'NON-PRINTABLE';
                        charClass += ' char-special';
                    } else if (entry.character.startsWith("'") && entry.character.endsWith("'")) {
                        charDisplay = entry.character.slice(1, -1); // Remove quotes
                    }
                    break;
            }
            
            // Ensure bits is a valid number, fallback to code length
            const bits = entry.bits || (entry.code ? entry.code.length : 0);
            
            row.innerHTML = `
                <td>${entry.index}</td>
                <td><span class="${charClass}">${charDisplay}</span></td>
                <td><span class="frequency-number">${entry.frequency.toLocaleString()}</span></td>
                <td><span class="code-display">${entry.code}</span></td>
                <td><span class="bits-number">${bits}</span></td>
            `;
            
            frequencyTableBody.appendChild(row);
        });
    }
    
    function sortTable(column) {
        // Toggle direction if clicking the same column
        if (currentSort.column === column) {
            currentSort.direction = currentSort.direction === 'asc' ? 'desc' : 'asc';
        } else {
            currentSort.column = column;
            currentSort.direction = 'asc';
        }
        
        // Sort the data
        tableData.sort((a, b) => {
            let valueA, valueB;
            
            switch (column) {
                case 'index':
                    valueA = a.index;
                    valueB = b.index;
                    break;
                case 'character':
                    // Sort by character, but put special characters first
                    valueA = a.character;
                    valueB = b.character;
                    break;
                case 'frequency':
                    valueA = a.frequency;
                    valueB = b.frequency;
                    break;
                case 'code':
                    // Sort by code length first, then lexicographically
                    if (a.code.length !== b.code.length) {
                        valueA = a.code.length;
                        valueB = b.code.length;
                    } else {
                        valueA = a.code;
                        valueB = b.code;
                    }
                    break;
                case 'bits':
                    valueA = a.bits || (a.code ? a.code.length : 0);
                    valueB = b.bits || (b.code ? b.code.length : 0);
                    break;
                default:
                    return 0;
            }
            
            let result;
            if (typeof valueA === 'number' && typeof valueB === 'number') {
                result = valueA - valueB;
            } else {
                result = String(valueA).localeCompare(String(valueB));
            }
            
            return currentSort.direction === 'asc' ? result : -result;
        });
        
        // Update header indicators
        const table = frequencyTableSection.querySelector('.frequency-table');
        const headers = table.querySelectorAll('th.sortable');
        headers.forEach(header => {
            header.classList.remove('sort-asc', 'sort-desc');
            if (header.dataset.column === column) {
                header.classList.add(currentSort.direction === 'asc' ? 'sort-asc' : 'sort-desc');
            }
        });
        
        // Re-render table
        renderTable(tableData);
    }
    
    // Add click listeners to sortable headers
    const table = frequencyTableSection.querySelector('.frequency-table');
    const sortableHeaders = table.querySelectorAll('th.sortable');
    sortableHeaders.forEach(header => {
        header.addEventListener('click', () => {
            sortTable(header.dataset.column);
        });
    });
    
    // Initial render (sorted by index by default)
    renderTable(tableData);
    
    // Show the frequency table section
    frequencyTableSection.style.display = 'block';
    
    // Add animation
    setTimeout(() => {
        frequencyTableSection.classList.add('fade-in');
    }, 100);
}
