#include <iostream>
#include <string>
#include <vector>
#include <stack>
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
 * For node traversal purposes
*/
class Node {
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

        // Maximum number of keys that a LeafNode or NonLeafNode can hold
        static const int n = 10; // TODO: Change once this can be calculated

        // Return height of tree
        int getTreeHeight() {
            if (root == nullptr) {
                // Empty tree
                return 0;
            } else {
                // Recursively fetch one level below until leaf node is reached

                int height = 1;
                Node* cur = root;

                // Check if cur is a LeafNode. If not, check downwards 
                NonLeafNode* nonLeafNode = dynamic_cast<NonLeafNode*>(cur);
                while (nonLeafNode != nullptr) {
                    cur = nonLeafNode->ptrArray[0]; // First pointer of node
                    nonLeafNode = dynamic_cast<NonLeafNode*>(cur);
                    height++;
                }

                return height;
            }
        }

        // Search for exact match of key
        vector<string*> exactSearch(string key) {
            Node* cur = getLeftmostLeafNodeToSearch(key);

            // For each exact match, store the resulting pointer
            vector<string*> results;
            LeafNode* leafNode = dynamic_cast<LeafNode*>(cur);
            bool isSearching = true; // keep track of whether the next LeafNode still needs to be searched or not

            // Continue looping until reached last LeafNode or key is greater than target
            while (isSearching || leafNode != nullptr)
            {
                // Loop through one LeafNode
                for (KeyPointerPair kpp : leafNode->kppArray) {
                    
                    // Check if exact match
                    if (key == kpp.key) {
                        results.push_back(kpp.record);
                    } else if (key > kpp.key) {

                        // The rest of the keys are greater than the target
                        // No need to search anymore
                        isSearching = false;
                        break;
                    } 
                    // Otherwise, key is smaller than target, or key is empty.
                    // Continue searching the next LeafNode
                }

                // Fetch next LeafNode in linked list to continue searching
                cur = leafNode->nextNode;
                leafNode = dynamic_cast<LeafNode*>(cur);
            }
            
            return results;
        }

        // Search for key within a range of values
        vector<string*> rangeSearch(int low, int high) {
            Node* cur = getLeftmostLeafNodeToSearch(to_string(low));

            // For each exact match, store the resulting pointer
            vector<string*> results;
            LeafNode* leafNode = dynamic_cast<LeafNode*>(cur);
            bool isSearching = true; // keep track of whether the next LeafNode still needs to be searched or not

            // Continue looping until reached last LeafNode or key is greater than upper bound
            while (isSearching || leafNode != nullptr)
            {
                // Loop through one LeafNode
                for (KeyPointerPair kpp : leafNode->kppArray) {
                    
                    // Check if within range
                    if (low < stoi(kpp.key) && high > stoi(kpp.key)) {
                        results.push_back(kpp.record);
                    } else if (high < stoi(kpp.key)) {

                        // The rest of the keys are greater than the upper bound
                        // No need to search anymore
                        isSearching = false;
                        break;
                    } 
                    // Otherwise, key is smaller than upper bound, or key is empty.
                    // Continue searching the next LeafNode
                }

                // Fetch next LeafNode in linked list to continue searching
                cur = leafNode->nextNode;
                leafNode = dynamic_cast<LeafNode*>(cur);
            }
            
            return results;
        }

        // Return total number of LeafNodes and NonLeafNodes
        int getNumNodes() {
            // Use DFS to traverse through all nodes
            int numNodes = 0;
            stack<Node*> nodeStack;
            nodeStack.push(root); // Start with the root node

            // Begin DFS
            Node* cur;
            while (!nodeStack.empty()) {
                // Pop a value from the stack
                cur = nodeStack.top();
                nodeStack.pop();
                numNodes++;

                // Process the current Node
                // Check if this is a LeafNode
                LeafNode* leafNode = dynamic_cast<LeafNode*>(cur);
                if(leafNode != nullptr) {
                    // Nothing more needs to be done, continue the while loop
                    continue;
                }

                // Otherwise, this should be a NonLeafNode
                NonLeafNode* nonLeafNode = dynamic_cast<NonLeafNode*>(cur);
                if(nonLeafNode != nullptr) {
                    // Add all Node pointers to the stack
                    for (Node* ptr : nonLeafNode->ptrArray) {
                        if (ptr != nullptr) {
                            nodeStack.push(ptr);
                        }
                    }
                }
            }

            return numNodes;
        }

        /**
         * Returns a vector array of a standard array of key values.
         * Each element in the vector array corresponds to one LeafNode.
         * Each element in the standard array corresponds to one key.
        */
        vector<string[BPTree::n]> displayLeafNodes() {
            vector<string[BPTree::n]> results;
            Node* cur = getLeftmostLeafNodeToSearch(""); // go to leftmost LeafNode directly

            LeafNode* leafNode = dynamic_cast<LeafNode*>(cur);
            do {
                string keysOfNode[BPTree::n]; // keys for this node

                // Traverse through one whole Node
                for (int i = 0; i < BPTree::n; i++) {
                    string key = leafNode->kppArray[i].key;
                    if (key != "") {
                        // Add all non-null keys into temporary string array
                        keysOfNode[i] = key;
                    } else {
                        break;
                    }
                }

                // Add this string array into result
                results.push_back(keysOfNode);

                // Traverse to next LeafNode in linked list
                cur = leafNode->nextNode;
                leafNode = dynamic_cast<LeafNode*>(cur);

            } while (cur != nullptr);
        }

    private:
        /**
         * Helper function for search() functions
         * 
         * Perform comparisons and tree traversals
         * to reach the leftmost LeafNode to begin
         * indexing search results.
        */
        Node* getLeftmostLeafNodeToSearch(string key) {
            Node* cur = root;

            // Check if cur is a LeafNode. If not, check downwards 
            NonLeafNode* nonLeafNode = dynamic_cast<NonLeafNode*>(cur);
            while (nonLeafNode != nullptr) {

                // Determine the next node to go downwards
                int index = 0;
                for (string i : nonLeafNode->keyArray) {
                    if (key > i) {
                        index++;
                    } else {
                        break;
                    }
                }
                cur = nonLeafNode->ptrArray[index];
                
                // Check while loop condition
                nonLeafNode = dynamic_cast<NonLeafNode*>(cur);
            } 

            return cur;
        }
};