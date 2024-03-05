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
        vector<tuple<int, int>> exactSearch(int key);

        // Search for key within a range of values
        vector<tuple<int, int>> rangeSearch(int low, int high);

        /**
         * Return number of non-leaf nodes scanned
         * To be used together with either exactSearch() or rangeSearch()
         * For rangeSearch(), use the argument fow 'low' as the argument for this function
        */
        int getNumIndexNodes(int key);

        // Return total number of LeafNodes and NonLeafNodes
        int getTotalNumNodes();

        /**
         * Prints out all of the leaf nodes
        */
        void displayLeafNodes();

        // Prints out the keys of the root node
        void displayRootNode();

        // Insert a new key into the B+ tree
        void insertKey(int key, int blockId, int blockOffset);

        // Delete a key from the B+ tree
        void deleteKey(int key);
  
    private:
        /**
         * Helper function
         * 
         * Perform comparisons and tree traversals to reach the
         * desired leaf node.
         * 
         * Most of the time, this method performs within the expected
         * behaviour: to reach the correct leaf node that contains
         * the key to search for. However, if there are multiple duplicate
         * keys in a row that spans multiple leaf nodes, then the behaviour
         * of this method needs to differ:
         * 
         * 1. If this method is used for search-related functions, this
         * function should return the leftmost node containing the given
         * key. This is so that the search can be done left-to-right.
         * 
         * 2. If this method is used for insert-related functions, this
         * function should return the rightmost node containing the given
         * key. This is so that overflow of duplicate keys occur to the 
         * right of the parent key instead of to the left, to maintain
         * ascending order of keys in leaf nodes.
         * 
         * @param key The key value to retrieve the node of
         * @param insert Specifies the context of the function. Set to true
         * if this is called in an insert-related function. Set to false if
         * this is called in a search-related function
        */
        Node* getLeafNode(int key, bool insert);

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
        void insertInternalNode(int key, vector<NonLeafNode*> nodePath, Node* nextPtr);

        //get num keys in leaf node
        int getNumKeys(LeafNode* node);

        int getNumKeysNL(NonLeafNode* node);

        //remove internal nodes in tree.
        void removeInternalNode(int key,NonLeafNode *parent,Node *child);

        //update parent node key with key of child node
        void updateParentKey(int prevIndex,Node *parent,Node *child,std::vector<NonLeafNode*> &path, std::vector<int> &pathIndexes);
};