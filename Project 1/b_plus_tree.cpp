#include <iostream>
#include <string>
using namespace std;

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
        KeyPointerPair() : key(0), record(nullptr) {}

        // Constructor initializing all attributes
        KeyPointerPair(string key, string* record) : key(key), record(record) {}
};

/**
 * Virtual base class for LeafNode and NonLeafNode
 * 
 * These two classes have the same number of keys and
 * same number of pointers. This class is designed to express
 * these similarities
*/
class Node {
    protected:
        // Return the index-th occurence of a key value
        virtual string getKey(int index);
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
        KeyPointerPair kppArray[BPTree::n]; 

        // Reference to the next LeafNode in the linked list
        LeafNode* nextNode;

        // Constructor initializing all attributes
        LeafNode(KeyPointerPair initialKpp[BPTree::n], LeafNode* nextNode) {
            // Copy the values from argument to attribute
            for (int i = 0; i < BPTree::n; ++i) {
                kppArray[i] = initialKpp[i];
            }

            this->nextNode = nextNode;
        };

        string getKey(int index) override {
            return kppArray[index].key;
        };
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
        Node* ptrArray[BPTree::n + 1];

        // Stores an array of keys
        string keyArray[BPTree::n];

        // Constructor initializing all attributes
        NonLeafNode(Node* initialPtrArray[BPTree::n + 1], string initialKeyArray[BPTree::n]) {
            // Copy the values from argument to attribute
            for (int i = 0; i < BPTree::n + 1; ++i) {
                ptrArray[i] = initialPtrArray[i];
            }
            for (int i = 0; i < BPTree::n; ++i) {
                keyArray[i] = initialKeyArray[i];
            }
        }

        string getKey(int index) override {
            return keyArray[index];
        }
};

/**
 * Stores a reference to one instance of an entire B+ tree
*/
class BPTree {
    public:
        // Maximum number of keys that a LeafNode or NonLeafNode can hold
        static const int n = 10; // TODO: Change array size once this can be calculated

        /**
         * Stores the highest level node.
         * If the tree consists of only LeafNode, the root node is the leftmost node
        */
        Node* root = nullptr;
};