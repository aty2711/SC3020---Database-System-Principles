#include <iostream>
#include <stack>
#include "b_plus_tree.h"
#include "tree_helper.h"
using namespace std;

int BPTree::getTreeHeight() {
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

vector<string*> BPTree::exactSearch(double key) {
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

vector<string*> BPTree::rangeSearch(double low, double high) {
    Node* cur = getLeftmostLeafNodeLessThan(low);

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
            if (low < kpp.key && high > kpp.key) {
                results.push_back(kpp.record);
            } else if (high < kpp.key) {

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

int BPTree::getNumNodes() {
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

            // For debugging purposes
            // Dump all of the keys in this node

            // Traverse through one whole Node
            double keysOfNode[n];
            double key;
            for (int i = 0; i < n; i++) {
                key = nonLeafNode->keyArray[i];
                if (key != nulldouble) {
                    // Add all non-null keys into temporary string array
                    keysOfNode[i] = key;
                } else {
                    keysOfNode[i] = nulldouble;
                }
            }

            // Print these keys out
            cout << "(";
            int length = sizeof(keysOfNode)/sizeof(keysOfNode[0]);
            for (int i = 0; i < length; i++) {
                if (keysOfNode[i] != nulldouble) {
                    cout << keysOfNode[i];
                }

                // Print a comma
                if (i != length - 1) {
                    cout << ",";
                }
            }
            cout << ") & ";
        }
    }

    return numNodes;
}

void BPTree::displayLeafNodes() {
    Node* cur = getLeftmostLeafNodeLessThan(nulldouble); // go to leftmost LeafNode directly

    LeafNode* leafNode = dynamic_cast<LeafNode*>(cur);
    do {
        double keysOfNode[n]; // keys for this node

        // Traverse through one whole Node
        for (int i = 0; i < n; i++) {
            double key = leafNode->kppArray[i].key;
            if (key != nulldouble) {
                // Add all non-null keys into temporary string array
                keysOfNode[i] = key;
            } else {
                keysOfNode[i] = nulldouble;
            }
        }

        // Print these keys out
        cout << "(";
        int length = sizeof(keysOfNode)/sizeof(keysOfNode[0]);
        for (int i = 0; i < length; i++) {
            if (keysOfNode[i] != nulldouble) {
                cout << keysOfNode[i];
            }

            // Print a comma
            if (i != length - 1) {
                cout << ",";
            }
        }
        cout << ") -> ";

        // Traverse to next LeafNode in linked list
        cur = leafNode->nextNode;
        leafNode = dynamic_cast<LeafNode*>(cur);

    } while (cur != nullptr);
}

void BPTree::insertKey(double key, string* record) {
    // If the B+ tree is empty, create a new LeafNode and insert there
    if (root == nullptr) {
        // Create new LeafNode
        LeafNode* newLeafNode = new LeafNode();
        newLeafNode->kppArray[0].key = key;
        newLeafNode->kppArray[0].record = record;

        // Assign root to new LeafNode
        root = newLeafNode;

        return;
    }

    // Check where to insert the key
    LeafNode* targetNode = dynamic_cast<LeafNode*>(getLeftmostLeafNodeLessThan(key));

    // Check whether the target node is already full
    bool isFull = true;
    for (KeyPointerPair kpp : targetNode->kppArray) {
        // isFull will be set to false if at least one key is missing
        if (kpp.key == nulldouble) {
            isFull = false;
            break;
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
        if (!isInserted) {
            // The new key is bigger than all other keys
            KeyPointerPair newKpp = KeyPointerPair(key, record);
            tempKpps[tempKppsIndex++] = newKpp;
            isInserted = true;
        }

        // Determine the middle element of the temp list
        // Index is half rounded down
        // Middle element will be present in parent node
        int middleIndex = floor((n + 1) / 2);
        KeyPointerPair middleKpp = tempKpps[middleIndex];

        // Split the LeafNode into two LeafNodes
        LeafNode* newLeafNode = new LeafNode();
        int nodeIndex = 0;
        for(int i = middleIndex; i < n + 1; i++) {
            // Insert middle element onwards to new LeafNode
            newLeafNode->kppArray[nodeIndex].key = tempKpps[i].key;
            newLeafNode->kppArray[nodeIndex].record = tempKpps[i].record;
            nodeIndex++;
        }
        for(int i = 0; i < n; i++) {
            // Empty the target node
            targetNode->kppArray[i].key = nulldouble;
            targetNode->kppArray[i].record = nullptr;
        }
        for(int i = 0; i < middleIndex ; i++) {
            // Rewrite the elements in the target LeafNode
            targetNode->kppArray[i].key = tempKpps[i].key;
            targetNode->kppArray[i].record = tempKpps[i].record;
        }
        
        // Reassign pointer of the target LeafNode and new LeafNode
        newLeafNode->nextNode = targetNode->nextNode;
        targetNode->nextNode = newLeafNode;

        // Determine the parent of the target node
        vector<NonLeafNode*> nodePath = getNodePath(middleKpp);
        if (nodePath.size() == 0) {
            // Node currently has no parent

            // Find the attributes required to create a new
            // NonLeafNode instance as parent node
            NonLeafNode* parentNode = new NonLeafNode();
            parentNode->ptrArray[0] = targetNode;
            parentNode->keyArray[0] = middleKpp.key;

            // The pointer after the key in the parent node
            // Should point to the newly created LeafNode
            parentNode->ptrArray[1] = newLeafNode;

            // Set parent node as root node
            root = parentNode;
        } else {
            // Node has non-empty parent
            // Insert new key into parent
            insertIntoNonLeafNodes(middleKpp.key, nodePath, newLeafNode);
        }
    } else {
        // Node is not yet full
        // Insert the key into the right place, and then push all other
        // KeyPointerPairs backwards
        int targetIndex = 0;
        for (KeyPointerPair kpp : targetNode->kppArray) {

            // Find the first key greater than the inserting key
            // Or empty key
            if (key < kpp.key || kpp.key == nulldouble) {
                break;
            }
            targetIndex++;
        }

        // Push all of the KeyPointerPairs back until the targetIndex
        for (int i = n - 2; i >= targetIndex; i--) {
            targetNode->kppArray[i+1] = targetNode->kppArray[i];
        }

        // Insert the KeyPointerPair into the empty slot
        targetNode->kppArray[targetIndex] = KeyPointerPair(key, record);
    }
    
}

Node* BPTree::getLeftmostLeafNodeLessThan(double key) {
    Node* cur = root;

    // Check if cur is a LeafNode. If not, check downwards 
    NonLeafNode* nonLeafNode = dynamic_cast<NonLeafNode*>(cur);
    while (nonLeafNode != nullptr) {

        // Determine the next node to go downwards
        int index = 0;
        for (double i : nonLeafNode->keyArray) {
            if (key > i && i != nulldouble) {
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

vector<NonLeafNode*> BPTree::getNodePath(KeyPointerPair kpp) {
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
        for (double i : nonLeafNode->keyArray) {
            if (kpp.key > i && i != nulldouble) {
                index++;
            } else {
                break;
            }
        }
        cur = nonLeafNode->ptrArray[index];

        // Add node to path
        nodePath.push_back(nonLeafNode);
        
        // Check while loop condition
        nonLeafNode = dynamic_cast<NonLeafNode*>(cur);
    } 

    return nodePath;
}

void BPTree::insertIntoNonLeafNodes(double key, vector<NonLeafNode*> nodePath, Node* nextPtr) {
    // Retrieve current NonLeafNode which is right above the previous
    // NonLeafNode that was being inspected
    NonLeafNode* cur = nodePath.back();
    nodePath.pop_back(); // in case of future calls of this function

    // Check whether this node is already full
    bool isFull = true;
    for (double key : cur->keyArray) {
        // isFull will be set to false if at least one key is missing
        if (key == nulldouble) {
            isFull = false;
            break;
        }
    }

    if(isFull) {
        // If current node is full, then need to involve parent node in insertion

        // Create a sorted temporary list of keys
        double tempKeys[n + 1];
        int tempKeysIndex = 0;
        int insertPtrIndex = 0; // Track where the pointer should be inserted
        bool isInserted = false; // Check if new key is already inserted
        for (double curKey : cur->keyArray) {
            if (key < curKey && !isInserted) {
                tempKeys[tempKeysIndex++] = key;
                insertPtrIndex = tempKeysIndex;
                isInserted = true;
            }
            tempKeys[tempKeysIndex++] = curKey;
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
        double middleKey = tempKeys[middleIndex];

        // Split the NonLeafNode into two NonLeafNodes
        NonLeafNode* newNonLeafNode = new NonLeafNode();
        int nodeIndex = 0;
        for(int i = middleIndex + 1; i < n + 1; i++) {
            // Insert middle element + 1 key onwards to new NonLeafNode
            newNonLeafNode->keyArray[nodeIndex++] = tempKeys[i];
        }
        nodeIndex = 0;
        for(int i = middleIndex + 1; i < n + 2; i++) {
            // Insert middle element + 1 pointer onwards to new NonLeafNode
            newNonLeafNode->ptrArray[nodeIndex++] = tempPtrs[i];
        }
        for(int i = 0; i < n; i++) {
            // Empty all keys from current node
            cur->keyArray[i] = nulldouble;
        }
        for(int i = 0; i < n + 1; i++) {
            // Empty all pointers from current node
            cur->ptrArray[i] = nullptr;
        }
        for(int i = 0; i < middleIndex; i++) {
            // Rewrite the keys in the current node
            cur->keyArray[i] = tempKeys[i];
        }
        for(int i = 0; i < middleIndex + 1; i++) {
            // And also rewrite pointers for current node
            cur->ptrArray[i] = tempPtrs[i];
        }

        // Determine the parent of the target node
        if (nodePath.size() == 0) {
            // Node currently has no parent

            // Find the attributes required to create a new
            // NonLeafNode instance as parent node
            NonLeafNode* parentNode = new NonLeafNode();
            parentNode->ptrArray[0] = cur;
            parentNode->keyArray[0] = middleKey;

            // The pointer after the key in the parent node
            // Should point to the newly created NonLeafNode
            parentNode->ptrArray[1] = newNonLeafNode;

            // Set the root to this parent
            root = parentNode;
        } else {
            // Node has non-empty parent
            // Insert new key into parent
            insertIntoNonLeafNodes(middleKey, nodePath, newNonLeafNode);
        }
    } else {
        // Node is not yet full
        // Insert the key into the right place, and then push all other
        // keys and pointers backwards
        int targetIndex = 0;
        for (double curKey : cur->keyArray) {

            // Find the first key greater than the inserting key
            // Or empty key
            if (key < curKey || curKey == nulldouble) {
                break;
            }
            targetIndex++;
        }

        // Push all of the pointers back until after the targetIndex
        for (int i = n - 1; i >= targetIndex + 1; i--) {
            cur->ptrArray[i+1] = cur->ptrArray[i];
        }
        // Push all of the keys back until the targetIndex
        for (int i = n - 2; i >= targetIndex; i--) {
            cur->keyArray[i+1] = cur->keyArray[i];
        }

        // Insert the key into the empty slot
        cur->keyArray[targetIndex] = key;
        cur->ptrArray[targetIndex + 1] = nextPtr;
    }
}