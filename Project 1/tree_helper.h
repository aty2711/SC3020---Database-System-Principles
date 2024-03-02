#pragma once // Header guard to prevent multiple inclusions
#include <string>
using namespace std;

// Maximum number of keys that a LeafNode or NonLeafNode can hold
const int n = 10; // TODO: Change once this can be calculated

// Integer value to indicate the key is empty
const double nullInt = -1;

/**
 * Stores a reference to one pair of record key and pointer to
 * the record in the leaf node. 
 * 
 * An instance of this class is designed to be unique, because 
 * value of keys can be repeated. Therefore, do not use this in non-leaf nodes
 * 
 * A visualization of an instance of this class will look like this:
 * KeyPointerPair [ pointer_to_record | key ]
*/
class KeyPointerPair {
    public:
        // Key of the record
        int key;

        // Reference to the data record
        int blockId;
        int blockOffset;

        // Default constructor
        KeyPointerPair();

        // Constructor initializing all attributes
        KeyPointerPair(int key, int blockId, int blockOffset);
};

/**
 * Virtual base class for LeafNode and NonLeafNode
 * 
 * For node traversal purposes
*/
class Node {
    public:
        virtual ~Node() {}
};

/**
 * Stores a reference to one leaf node within a B+ tree
 * 
 * A visualization of an instance of this class will look like this:
 * (kpp is an instance of KeyPointerPair)
 * Leaf Node [ kpp_0 | kpp_1 | ... | kpp_n | pointer_to_next_node]
*/
class LeafNode : public Node {
    public: 
        // Stores an array of KeyPointerPair classes
        KeyPointerPair kppArray[n]; 

        // Reference to the next LeafNode in the linked list
        LeafNode* nextNode;

        // Default constructor
        LeafNode();
};

/**
 * Stores a reference to one non-leaf node or internal node within a B+ tree
 * 
 * A visualization of an instance of this class will look like this:
 * Non-Leaf Node [ ptr_0 | key_0 | ptr_1 | key_1 | ... | ptr_n | key_n | ptr_n+1 ]
*/
class NonLeafNode : public Node{
    public:
        // Stores an array of pointers to LeafNodes or NonLeafNodes
        Node* ptrArray[n + 1];

        // Stores an array of keys
        int keyArray[n];

        // Default constructor
        NonLeafNode();
};
