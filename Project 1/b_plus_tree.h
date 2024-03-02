#pragma once // Header guard to prevent multiple inclusions
#include <string>
#include <vector>
#include "tree_helper.h"
using namespace std;

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
        vector<string*> exactSearch(double key);

        // Search for key within a range of values
        vector<string*> rangeSearch(double low, double high);

        // Return total number of LeafNodes and NonLeafNodes
        int getNumNodes();

        /**
         * Prints out all of the leaf nodes
        */
        void displayLeafNodes();

        // Insert a new key into the B+ tree
        void insertKey(double key, string* record);

    private:
        /**
         * Helper function
         * 
         * Perform comparisons and tree traversals
         * to reach the leftmost LeafNode to begin
         * indexing search results.
        */
        Node* getLeftmostLeafNodeLessThan(double key);

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
        void insertIntoNonLeafNodes(double key, vector<NonLeafNode*> nodePath, Node* nextPtr);
};