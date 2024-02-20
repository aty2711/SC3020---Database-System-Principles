#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stack>
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
        KeyPointerPair() : key(""), record(nullptr) {}

        // Constructor initializing all attributes
        KeyPointerPair(string key, string* record) : key(key), record(record) {}
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
        LeafNode() {
            for (int i = 0; i < n; i++) {
                kppArray[i] = KeyPointerPair();
            }

            nextNode = nullptr;
        }

        // Constructor initializing all attributes
        LeafNode(KeyPointerPair initialKpp[n], LeafNode* nextNode) {
            // Copy the values from argument to attribute
            for (int i = 0; i < n; ++i) {
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
        Node* ptrArray[n + 1];

        // Stores an array of keys
        string keyArray[n];

        // Default constructor
        NonLeafNode() {
            for (int i = 0; i < n; i++) {
                keyArray[i] = "";
            }
            for (int i = 0; i < n + 1; i++) {
                ptrArray[i] = nullptr;
            }
        }

        // Constructor initializing all attributes
        NonLeafNode(Node* initialPtrArray[n + 1], string initialKeyArray[n]) {
            // Copy the values from argument to attribute
            for (int i = 0; i < n + 1; ++i) {
                ptrArray[i] = initialPtrArray[i];
            }
            for (int i = 0; i < n; ++i) {
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
            Node* cur = getLeftmostLeafNodeLessThan(key);

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
            Node* cur = getLeftmostLeafNodeLessThan(to_string(low));

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
        vector<string[n]> displayLeafNodes() {
            vector<string[n]> results;
            Node* cur = getLeftmostLeafNodeLessThan(""); // go to leftmost LeafNode directly

            LeafNode* leafNode = dynamic_cast<LeafNode*>(cur);
            do {
                string keysOfNode[n]; // keys for this node

                // Traverse through one whole Node
                for (int i = 0; i < n; i++) {
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

            return results;
        }

        // Insert a new key into the B+ tree
        void insertKey(string key, string* record) {
            // If the B+ tree is empty, create a new LeafNode and insert there
            if (root == nullptr) {
                // Create new LeafNode
                LeafNode newLeafNode = LeafNode();
                newLeafNode.kppArray[0].key = key;
                newLeafNode.kppArray[0].record = record;

                // Assign root to new LeafNode
                root = &newLeafNode;

                return;
            }

            // Check where to insert the key
            LeafNode* targetNode = dynamic_cast<LeafNode*>(getLeftmostLeafNodeLessThan(key));

            // Check whether the target node is already full
            bool isFull = true;
            for (KeyPointerPair kpp : targetNode->kppArray) {
                // isFull will be set to false if at least one key is missing
                if (kpp.key == "") {
                    isFull = false;
                    return;
                }
            }

            if(isFull) {
                // If LeafNode is full, then need to involve parent node in insertion

                // Create a sorted temporary list of KeyPointerPairs
                KeyPointerPair tempKpps[n + 1];
                int tempKppsIndex = 0;
                bool isInserted = false; // Check if new key is already inserted
                for (KeyPointerPair kpp : targetNode->kppArray) {
                    if (key < kpp.key && !isInserted) {
                        KeyPointerPair newKpp = KeyPointerPair(key, record);
                        tempKpps[tempKppsIndex++] = newKpp;
                        isInserted = true;
                    }
                    tempKpps[tempKppsIndex++] = kpp;
                }

                // Determine the middle element of the temp list
                // Index is half rounded down
                // Middle element will be present in parent node
                int middleIndex = floor((n + 1) / 2);
                KeyPointerPair middleKpp = tempKpps[middleIndex];

                // Split the LeafNode into two LeafNodes
                LeafNode newLeafNode = LeafNode();
                for(int i = middleIndex; i < n; i++) {
                    // Insert middle element onwards to new LeafNode
                    newLeafNode.kppArray[i].key = tempKpps[i].key;
                    newLeafNode.kppArray[i].record = tempKpps[i].record;
                }
                for(int i = middleIndex + 1; i < n; i++) {
                    // Then, empty all elements transferred to new LeafNode
                    targetNode->kppArray[i].key = "";
                    targetNode->kppArray[i].record = nullptr;
                }
                
                // Reassign pointer of the target LeafNode and new LeafNode
                newLeafNode.nextNode = targetNode->nextNode;
                targetNode->nextNode = &newLeafNode;

                // Determine the parent of the target node
                vector<NonLeafNode*> nodePath = getNodePath(middleKpp);
                if (nodePath.size() == 0) {
                    // Node currently has no parent

                    // Find the attributes required to create a new
                    // NonLeafNode instance as parent node
                    NonLeafNode parentNode = NonLeafNode();
                    parentNode.ptrArray[0] = targetNode;
                    parentNode.keyArray[0] = middleKpp.key;

                    // The pointer after the key in the parent node
                    // Should point to the newly created LeafNode
                    parentNode.ptrArray[1] = &newLeafNode;
                } else {
                    // Node has non-empty parent
                    // Insert new key into parent
                    insertIntoNonLeafNodes(middleKpp.key, nodePath, &newLeafNode);
                }
            } else {
                // Node is not yet full
                // Insert the key into the right place, and then push all other
                // KeyPointerPairs backwards
                int targetIndex = 0;
                for (KeyPointerPair kpp : targetNode->kppArray) {

                    // Find the first key greater than the inserting key
                    if (key > kpp.key) {
                        return;
                    }
                    targetIndex++;
                }

                // Push all of the KeyPointerPairs back until the targetIndex
                for (int i = n - 2; i >= targetIndex; i++) {
                    targetNode->kppArray[i+1] = targetNode->kppArray[i];
                }

                // Insert the KeyPointerPair into the empty slot
                targetNode->kppArray[targetIndex] = KeyPointerPair(key, record);
            }
            
        }

    private:
        /**
         * Helper function
         * 
         * Perform comparisons and tree traversals
         * to reach the leftmost LeafNode to begin
         * indexing search results.
        */
        Node* getLeftmostLeafNodeLessThan(string key) {
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

        /**
         * Helper function for insertKey()
         * 
         * Find all nodes that need to be traversed in order to reach
         * the given KeyPointerPair. 
         * The traversed nodes will not include the LeafNode containing 
         * the KeyPointerPair.
        */
        vector<NonLeafNode*> getNodePath(KeyPointerPair kpp) {
            vector<NonLeafNode*> nodePath;

            // If the B+ tree has no NonLeafNode layer, return null pointer
            NonLeafNode* nonLeafNode = dynamic_cast<NonLeafNode*>(root);
            if(nonLeafNode == nullptr) {
                return nodePath;
            }

            // Check downwards 
            Node* cur = root;
            while (nonLeafNode != nullptr) {

                // Determine the next node to go downwards
                int index = 0;
                for (string i : nonLeafNode->keyArray) {
                    if (kpp.key > i) {
                        index++;
                    } else {
                        break;
                    }
                }
                cur = nonLeafNode->ptrArray[index];

                // Add node to path
                nodePath.push_back(nonLeafNode);
                
                // Check while loop condition
                cur = dynamic_cast<NonLeafNode*>(cur);
            } 

            return nodePath;
        }

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
        void insertIntoNonLeafNodes(string key, vector<NonLeafNode*> nodePath, Node* nextPtr) {
            // Retrieve current NonLeafNode which is right above the previous
            // NonLeafNode that was being inspected
            NonLeafNode* cur = nodePath.back();
            nodePath.pop_back(); // in case of future calls of this function

            // Check whether this node is already full
            bool isFull = true;
            for (string key : cur->keyArray) {
                // isFull will be set to false if at least one key is missing
                if (key == "") {
                    isFull = false;
                    return;
                }
            }

            if(isFull) {
                // If current node is full, then need to involve parent node in insertion

                // Create a sorted temporary list of keys
                string tempKeys[n + 1];
                int tempKeysIndex = 0;
                int insertPtrIndex = 0; // Track where the pointer should be inserted
                bool isInserted = false; // Check if new key is already inserted
                for (string i : cur->keyArray) {
                    if (key < i && !isInserted) {
                        tempKeys[tempKeysIndex++] = key;
                        insertPtrIndex = tempKeysIndex;
                        isInserted = true;
                    }
                    tempKeys[tempKeysIndex++] = i;
                }

                // Create a sorted temporary list of pointers
                Node* tempPtrs[n + 2];
                int tempPtrsIndex = 0;
                isInserted = false;
                for (int i = 0; i < n + 2; i++) {
                    if (i == insertPtrIndex) {
                        tempPtrs[tempPtrsIndex++] = nextPtr;
                        isInserted = true;
                    }
                    tempPtrs[tempPtrsIndex++] = cur->ptrArray[i];
                }


                // Determine the middle element of the temp list
                // Index is half rounded down
                // Middle element will be present in parent node
                int middleIndex = floor((n + 1) / 2);
                string middleKey = tempKeys[middleIndex];

                // Split the NonLeafNode into two NonLeafNodes
                NonLeafNode newNonLeafNode = NonLeafNode();
                for(int i = middleIndex + 1; i < n; i++) {
                    // Insert middle element + 1 key onwards to new NonLeafNode
                    newNonLeafNode.keyArray[i] = tempKeys[i];
                }
                for(int i = middleIndex + 1; i < n + 1; i++) {
                    // Insert middle element + 1 pointer onwards to new NonLeafNode
                    newNonLeafNode.ptrArray[i] = tempPtrs[i];
                }
                for(int i = middleIndex; i < n; i++) {
                    // Then, empty middle key and all keys transferred to new LeafNode
                    cur->keyArray[i] = "";
                }
                for(int i = middleIndex + 1; i < n; i++) {
                    // And empty all pointers transferred to new LeafNode
                    cur->ptrArray[i] = nullptr;
                }

                // Determine the parent of the target node
                if (nodePath.size() == 0) {
                    // Node currently has no parent

                    // Find the attributes required to create a new
                    // NonLeafNode instance as parent node
                    NonLeafNode parentNode = NonLeafNode();
                    parentNode.ptrArray[0] = cur;
                    parentNode.keyArray[0] = middleKey;

                    // The pointer after the key in the parent node
                    // Should point to the newly created NonLeafNode
                    parentNode.ptrArray[1] = &newNonLeafNode;
                } else {
                    // Node has non-empty parent
                    // Insert new key into parent
                    insertIntoNonLeafNodes(middleKey, nodePath, &newNonLeafNode);
                }
            } else {
                // Node is not yet full
                // Insert the key into the right place, and then push all other
                // keys and pointers backwards
                int targetIndex = 0;
                for (string i : cur->keyArray) {

                    // Find the first key greater than the inserting key
                    if (key > i) {
                        return;
                    }
                    targetIndex++;
                }

                // Push all of the pointers back until after the targetIndex
                for (int i = n - 1; i >= targetIndex + 1; i++) {
                    cur->ptrArray[i+1] = cur->ptrArray[i];
                }
                // Push all of the keys back until the targetIndex
                for (int i = n - 2; i >= targetIndex; i++) {
                    cur->keyArray[i+1] = cur->keyArray[i];
                }

                // Insert the key into the empty slot
                cur->keyArray[targetIndex] = key;
                cur->ptrArray[targetIndex + 1] = nextPtr;
            }
        }
};