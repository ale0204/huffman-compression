#pragma once

/**
 * @brief Node structure for building Huffman trees
 * 
 * Represents a single node in a Huffman tree. Can be either a leaf node
 * (containing a character and its frequency) or an internal node 
 * (containing only frequency and pointers to children).
 */
class HuffmanNode {
private:
    char character;      ///< Character to represent the node
    int frequency;       ///< Frequency representing how many times the character appears
                         ///< If frequency is 0, it means this node is not a leaf
    HuffmanNode* left;   ///< Pointer to left child node
    HuffmanNode* right;  ///< Pointer to right child node
    
public:
    /**
     * @brief Construct a new Huffman Node object
     * 
     * Creates a new node with the specified character and frequency.
     * Children pointers are initialized to nullptr.
     * 
     * @param ch Character to store in this node (default: '\0')
     * @param freq Frequency of the character (default: 0)
     */
    HuffmanNode(char ch = '\0', int freq = 0);
    
    /**
     * @brief Check if this node is a leaf node
     * 
     * A leaf node has no children (both left and right are nullptr).
     * Leaf nodes contain actual characters, while internal nodes only
     * contain frequency information.
     * 
     * @return bool True if this is a leaf node, false otherwise
     */
    bool isLeaf() const;
    
    /**
     * @brief Get the character stored in this node
     * @return char The character value
     */
    char getCharacter() const;
    
    /**
     * @brief Get the frequency of this node
     * @return int The frequency value
     */
    int getFrequency() const;
    
    /**
     * @brief Get pointer to left child
     * @return HuffmanNode* Pointer to left child (nullptr if no left child)
     */
    HuffmanNode* getLeft() const;
    
    /**
     * @brief Get pointer to right child
     * @return HuffmanNode* Pointer to right child (nullptr if no right child)
     */
    HuffmanNode* getRight() const;
    
    /**
     * @brief Set the left child pointer
     * @param node Pointer to the node to set as left child
     */
    void setLeft(HuffmanNode* node);
    
    /**
     * @brief Set the right child pointer
     * @param node Pointer to the node to set as right child
     */
    void setRight(HuffmanNode* node);
};

/**
 * @brief Comparator for HuffmanNode pointers in priority queues
 * 
 * Used to order nodes in a priority queue for Huffman tree construction.
 * Nodes with lower frequency have higher priority (min-heap behavior).
 */
struct NodeComparator {
    /**
     * @brief Compare two HuffmanNode pointers by frequency
     * 
     * @param a First node to compare
     * @param b Second node to compare
     * @return bool True if a has higher frequency than b (for min-heap)
     */
    bool operator()(HuffmanNode* a, HuffmanNode* b);
};
