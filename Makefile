# Makefile for Huffman Compression Utility
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DEBUG_DIR = $(BUILD_DIR)/debug
RELEASE_DIR = $(BUILD_DIR)/release

# Source files
SOURCES = main.cpp \
          $(SRC_DIR)/CommandLineOptions.cpp \
          $(SRC_DIR)/HuffmanException.cpp \
          $(SRC_DIR)/HuffmanNode.cpp \
          $(SRC_DIR)/ArchiveStructures.cpp \
          $(SRC_DIR)/HuffmanAlgorithm.cpp

# Object files
DEBUG_OBJECTS = $(SOURCES:%.cpp=$(DEBUG_DIR)/%.o)
RELEASE_OBJECTS = $(SOURCES:%.cpp=$(RELEASE_DIR)/%.o)

# Target executable
TARGET = huff
DEBUG_TARGET = $(DEBUG_DIR)/$(TARGET)
RELEASE_TARGET = $(RELEASE_DIR)/$(TARGET)

# Default target
.PHONY: all debug release clean help

all: release

debug: $(DEBUG_TARGET)

release: $(RELEASE_TARGET)

# Debug build
$(DEBUG_TARGET): $(DEBUG_OBJECTS) | $(DEBUG_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -o $@ $^
	@echo "Debug build completed: $@"

# Release build
$(RELEASE_TARGET): $(RELEASE_OBJECTS) | $(RELEASE_DIR)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -o $@ $^
	@echo "Release build completed: $@"

# Debug object files
$(DEBUG_DIR)/%.o: %.cpp | $(DEBUG_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -c $< -o $@

$(DEBUG_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp | $(DEBUG_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -c $< -o $@

# Release object files
$(RELEASE_DIR)/%.o: %.cpp | $(RELEASE_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(RELEASE_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp | $(RELEASE_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -c $< -o $@

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(DEBUG_DIR): | $(BUILD_DIR)
	mkdir -p $(DEBUG_DIR)
	mkdir -p $(DEBUG_DIR)/$(SRC_DIR)

$(RELEASE_DIR): | $(BUILD_DIR)
	mkdir -p $(RELEASE_DIR)
	mkdir -p $(RELEASE_DIR)/$(SRC_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	@echo "Build files cleaned"

# Install (copy to system directory - optional)
install: release
	cp $(RELEASE_TARGET) /usr/local/bin/$(TARGET)
	@echo "Installed $(TARGET) to /usr/local/bin/"

# Uninstall
uninstall:
	rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstalled $(TARGET)"

# Run tests (placeholder for future test implementation)
test: debug
	@echo "Running tests..."
	# Add test commands here when tests are implemented

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build release version (default)"
	@echo "  debug    - Build debug version with debugging symbols"
	@echo "  release  - Build optimized release version"
	@echo "  clean    - Remove all build files"
	@echo "  install  - Install release version to /usr/local/bin"
	@echo "  uninstall- Remove installed version"
	@echo "  test     - Run tests (debug version)"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Usage examples:"
	@echo "  make              # Build release version"
	@echo "  make debug        # Build debug version"
	@echo "  make clean        # Clean build files"
	@echo "  make install      # Build and install"

# Dependency tracking (automatic header dependency detection)
-include $(DEBUG_OBJECTS:.o=.d)
-include $(RELEASE_OBJECTS:.o=.d)

# Generate dependency files
$(DEBUG_DIR)/%.d: %.cpp | $(DEBUG_DIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM -MT $(DEBUG_DIR)/$*.o $< > $@

$(RELEASE_DIR)/%.d: %.cpp | $(RELEASE_DIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM -MT $(RELEASE_DIR)/$*.o $< > $@
