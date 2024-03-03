#pragma once // Header guard to prevent multiple inclusions
#include <string>
#include <vector>
using namespace std;

// Maximum number of keys that a LeafNode or NonLeafNode can hold
const int n = 10; // TODO: Change once this can be calculated

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
        string key;

        // Reference to the data record
        string* record; // TODO: Change this to the correct data type once data record is implemented

        // Default constructor
        KeyPointerPair();

        // Constructor initializing all attributes
        KeyPointerPair(string key, string* record);
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

        // Constructor initializing all attributes
        LeafNode(KeyPointerPair initialKpp[n], LeafNode* nextNode);
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
        string keyArray[n];

        // Default constructor
        NonLeafNode();

        // Constructor initializing all attributes
        NonLeafNode(Node* initialPtrArray[n + 1], string initialKeyArray[n]);
};

/**
 * Stores a reference to one instance of an entire B+ tree
*/
class BPTree {
    public:
        /**
         * Stores the highest level node.
         * If the tree consists of only LeafNode, the root node is the leftmost node
        */
        Node* root = nullptr;

        // Return height of tree
        int getTreeHeight();

        // Search for exact match of key
        vector<string*> exactSearch(string key);

        // Search for key within a range of values
        vector<string*> rangeSearch(int low, int high);

        // Return total number of LeafNodes and NonLeafNodes
        int getNumNodes();

        /**
         * Returns a vector array of a standard array of key values.
         * Each element in the vector array corresponds to one LeafNode.
         * Each element in the standard array corresponds to one key.
        */
        vector<string[n]> displayLeafNodes();

        // Insert a new key into the B+ tree
        void insertKey(string key, string* record);

        // Delete key in B+ tree
        void deleteKey(string key);

        //remove internal node in B+ tree
        void removeInternalNode(int x,Node *parent,Node *child);

        //update parent node keys 
        void updateParentKey(int prevIndex,Node *parent,Node *child,std::vector<NonLeafNode*> &path, std::vector<int> &pathIndexes);

        //get num keys in leaf node
        int getNumKeys(LeafNode* node);

        //get num keys in non leaf node
        int getNumKeysNL(NonLeafNode* node);

        //remove internal nodes in tree.
        void removeInternalNode(string x,NonLeafNode *parent,Node *child);

        //update parent node key with key of child node
        void updateParentKey(int prevIndex,Node *parent,Node *child,std::vector<NonLeafNode*> &path, std::vector<int> &pathIndexes);

    private:
        /**
         * Helper function
         * 
         * Perform comparisons and tree traversals
         * to reach the leftmost LeafNode to begin
         * indexing search results.
        */
        Node* getLeftmostLeafNodeLessThan(string key);

        /**
         * Helper function for insertKey()
         * 
         * Find all nodes that need to be traversed in order to reach
         * the given KeyPointerPair. 
         * The traversed nodes will not include the LeafNode containing 
         * the KeyPointerPair.
        */
        vector<NonLeafNode*> getNodePath(KeyPointerPair kpp);

        /**
         * Helper function for insertKey()
         * 
         * Recursively insert a new key into NonLeafNodes. 
         * If the NonLeafNode is full before insertion, run this function again.
         * 
         * @param key The key that needs to be inserted into a NonLeafNode
         * @param nodePath Path from root node to the current node
         * @param nextPtr After appending the key, this pointer will be at the right
         * of the key
        */
        void insertIntoNonLeafNodes(string key, vector<NonLeafNode*> nodePath, Node* nextPtr);
};