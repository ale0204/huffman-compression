#include "../include/HuffmanNode.h"

HuffmanNode::HuffmanNode(char ch, int freq) : 
    character(ch), frequency(freq), left(nullptr), right(nullptr) 
{
}

bool HuffmanNode::isLeaf() const 
{
    return left == nullptr && right == nullptr;
}

char HuffmanNode::getCharacter() const 
{
    return character;
}

int HuffmanNode::getFrequency() const 
{
    return frequency;
}

HuffmanNode* HuffmanNode::getLeft() const 
{
    return left;
}

HuffmanNode* HuffmanNode::getRight() const 
{
    return right;
}

void HuffmanNode::setLeft(HuffmanNode* node) 
{
    left = node;
}

void HuffmanNode::setRight(HuffmanNode* node) 
{
    right = node;
}

bool NodeComparator::operator()(HuffmanNode* a, HuffmanNode* b) 
{
    return a->getFrequency() > b->getFrequency();
}
