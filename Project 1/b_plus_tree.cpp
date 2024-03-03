#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stack>
#include "b_plus_tree.h"
using namespace std;

/*
~~~~~~~~~~~~~~~~~~~~~~~ KeyPointerPair ~~~~~~~~~~~~~~~~~~~~~~~~
*/

// Default constructor
KeyPointerPair::KeyPointerPair() : key(""), record(nullptr) {}

// Constructor initializing all attributes
KeyPointerPair::KeyPointerPair(string key, string* record) : key(key), record(record) {}

/*
~~~~~~~~~~~~~~~~~~~~~~~ LeafNode ~~~~~~~~~~~~~~~~~~~~~~~~
*/

// Default constructor
LeafNode::LeafNode() {
    for (int i = 0; i < n; i++) {
        kppArray[i] = KeyPointerPair();
    }

    nextNode = nullptr;
}

// Constructor initializing all attributes
LeafNode::LeafNode(KeyPointerPair initialKpp[n], LeafNode* nextNode) {
    // Copy the values from argument to attribute
    for (int i = 0; i < n; ++i) {
        kppArray[i] = initialKpp[i];
    }

    this->nextNode = nextNode;
};

/*
~~~~~~~~~~~~~~~~~~~~~~~ NonLeafNode ~~~~~~~~~~~~~~~~~~~~~~~~
*/

// Default constructor
NonLeafNode::NonLeafNode() {
    for (int i = 0; i < n; i++) {
        keyArray[i] = "";
    }
    for (int i = 0; i < n + 1; i++) {
        ptrArray[i] = nullptr;
    }
}

// Constructor initializing all attributes
NonLeafNode::NonLeafNode(Node* initialPtrArray[n + 1], string initialKeyArray[n]) {
    // Copy the values from argument to attribute
    for (int i = 0; i < n + 1; ++i) {
        ptrArray[i] = initialPtrArray[i];
    }
    for (int i = 0; i < n; ++i) {
        keyArray[i] = initialKeyArray[i];
    }
}

/*
~~~~~~~~~~~~~~~~~~~~~~~ BPTree ~~~~~~~~~~~~~~~~~~~~~~~~
*/
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

vector<string*> BPTree::exactSearch(string key) {
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

vector<string*> BPTree::rangeSearch(int low, int high) {
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
        }
    }

    return numNodes;
}

vector<string[n]> BPTree::displayLeafNodes() {
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

void BPTree::insertKey(string key, string* record) {
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

Node* BPTree::getLeftmostLeafNodeLessThan(string key) {
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

void BPTree::insertIntoNonLeafNodes(string key, vector<NonLeafNode*> nodePath, Node* nextPtr) {
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

void BPTree::deleteKey(string key){
    
    //Check if empty b tree
    if (this->root == nullptr){
        std::cout << "Empty tree" << std::endl;
        return;
    }

    //Search for Key 
    vector<string*> results = BPTree::exactSearch(key);
    
    //Key is not found
    if (results.empty()){
        std::cout << "Key" << key << "not found in tree" << std::endl;
    }

    Node *curNode = root;
    int index = 0;
    std::vector<NonLeafNode*> path; //vector to store path of traversal
    std::vector<int>pathIndexes;    //vector to store indexes of path of traversal
    int minKeys = (n+1)/2;  //max is 10

    //find target leaf node with key, save the path.
    NonLeafNode* nonLeafNode = dynamic_cast<NonLeafNode*>(curNode);
    while(nonLeafNode != nullptr){
        for (string i : nonLeafNode->keyArray){
            if (key > i) {
                index++;
            } else {
                break;
            }
        }
        
        path.push_back(nonLeafNode);
        pathIndexes.push_back(index);
        curNode = nonLeafNode->ptrArray[index];
        nonLeafNode = dynamic_cast<NonLeafNode*>(curNode);
    }

    LeafNode* targetNode = dynamic_cast<LeafNode*>(curNode);
    //find the key's index in leaf node, then delete the records
    //in target node, find same keys and delete the records
    int firstIndex = std::lower_bound(targetNode->kppArray->key.begin(), targetNode->kppArray->key.end(), key) - targetNode->kppArray->key.begin();

    for (int i=0 ; i<n ; i++){
        string nodeKey = targetNode->kppArray[i].key;
        if (nodeKey == key){
            //erase contens of kpp, including duplicates
            //targetNode->kppArray[i].key.erase();
            //(*targetNode->kppArray[i].record).erase();
            //dk which one works targetNode->kppArray[i].record->clear();
            targetNode->kppArray[i].key = "";
            targetNode->kppArray[i].record = nullptr;


            //delete key, then shift all the records 1 to the left
            for (int j=i+1 ; j < n ; j++){
                targetNode->kppArray[j-1].key = targetNode->kppArray[j].key;
                targetNode->kppArray[j-1].record = targetNode->kppArray[j].record;
            }

            //clear duplicate last key 
            targetNode->kppArray[getNumKeys(targetNode)-1].key = "";
            targetNode->kppArray[getNumKeys(targetNode)-1].record = nullptr;

            return;

        }

    }

 
    if (getNumKeys(targetNode) >= minKeys){

        //left most not removed
        if (targetNode->kppArray[0].key != ""){
            return;
        }
        
        //iterate backwards in path index,check parents nodes
        while (path.back() != NULL){
            //while 
            if (path.back()->keyArray[pathIndexes.back()] != key){
                pathIndexes.pop_back();
                path.pop_back();
            }

            else{
                //new leftmost key TODO check if key return is correct
                key = targetNode->kppArray->key.front();
                index = pathIndexes.back();

                curNode = path.back();
                //TODO see if work
                LeafNode* targetNode = dynamic_cast<LeafNode*>(curNode);
                targetNode->kppArray[index].key = key;
                return;
            }


        }
        
    }

    //2.Keys in leaf node < Min size
    else{

        int prevIndex = pathIndexes.back();
        Node *leftNode,*rightNode = nullptr; //left right pointers to keep track of neighbor
        NonLeafNode *parentNode = path.back();

        path.pop_back();
        //if there is a left neighbor of same parent node
        if(prevIndex > 0){
            leftNode = parentNode->ptrArray[prevIndex-1]; //left now points to node left of targetnode
            LeafNode* left = dynamic_cast<LeafNode*>(leftNode);
            
            //if can borrow from left, borrow
            if(getNumKeys(left) > minKeys){   
                // = last key in left node
                KeyPointerPair borrowleft = left->kppArray[getNumKeys(left) - 1];

                //remove last element of left node, doesnt reduce container size
                left->kppArray[getNumKeys(left) - 1].key = "";
                left->kppArray[getNumKeys(left) - 1].record = nullptr;
                
                

                //remove last element v 2
                //left->kppArray[getNumKeys(left)-1].key.erase();
                //(*left->kppArray[getNumKeys(left)-1].record).erase();

                //left->kppArray->key.pop_back();
                //(*left->kppArray->record).pop_back();

                
                //shift keys in target node by 1 to the right 
                for (int i = getNumKeys(targetNode); i>0; i--){
                    targetNode->kppArray[i] = targetNode->kppArray[i-1];
                }

                //insert into first element of target node
                targetNode->kppArray[0] = borrowleft;
                
                //targetNode->kppArray->key.insert(targetNode->kppArray->key.begin(), left->kppArray->key.back() );

                //update key in parent node to new left node added in target node
                parentNode->keyArray[prevIndex] = targetNode->kppArray[0].key;
                return;
                
            }
        }

        //check if theres a right neighbor
        if(prevIndex < parentNode->keyArray->size() - 1){
            rightNode = parentNode->ptrArray[prevIndex+1];
            LeafNode* right = dynamic_cast<LeafNode*>(rightNode);

            //if can borrow from right,borrow
            if(getNumKeys(right) > minKeys){
                //first key in right node
                KeyPointerPair borrowright = right->kppArray[0];

                //shift keys in right node by 1 to left
                for (int i = 1 ; i < n ; i++){
                    //right->kppArray[i] = right->kppArray[i+1];
                    right->kppArray[i-1].key = right->kppArray[i].key;
                    right->kppArray[i-1].record = right->kppArray[i].record;
                }

                //clear duplicate last key 
                right->kppArray[getNumKeys(right)-1].key = "";
                right->kppArray[getNumKeys(right)-1].record = nullptr;

                //insert into last element of target node
                targetNode->kppArray[getNumKeys(targetNode)] = borrowright;

                //update key in parent node 
                parentNode->keyArray[prevIndex] = right->kppArray[0].key;

                //check if deleted key in target node is the leftmost key , then need update parent key
                if (firstIndex == 0){
                    updateParentKey(prevIndex,targetNode,parentNode,path,pathIndexes);
                }
                //updateparentKey

            }


        }
        //merging 
        //there is left neighbor with  min keys,  merge 
        if (leftNode != nullptr){
            LeafNode*left = dynamic_cast<LeafNode*>(leftNode);
            //while no. of keys not equal to 0, put all keys from target node and merge into the left
            while (getNumKeys(targetNode) != 0){
                //set last element of left node to first element of target node
                left->kppArray[getNumKeys(left)-1] = targetNode->kppArray[0];

                //delete key in targetnode
                targetNode->kppArray[0].key = "";
                targetNode->kppArray[0].record = nullptr;

                //shift targetnode cells 1 to the left
                for(int i=1; i<getNumKeys(targetNode); i++){
                    targetNode->kppArray[i-1] = targetNode->kppArray[i];
                }

            }
            //update pointer to next node
            left->nextNode = targetNode->nextNode;
            //add remove internal
            removeInternalNode(parentNode->keyArray[prevIndex - 2], parentNode,targetNode);
        }


        //else, right neighbor has min keys, and merge
        else{
            LeafNode* right = dynamic_cast<LeafNode*>(rightNode);

            while(getNumKeys(right) != 0){
                //delete right records 

                //set last element of targetnode to first element of right
                targetNode->kppArray[getNumKeys(targetNode)-1] = right->kppArray[0];

                //delete key in rightnode
                right->kppArray[0].key = "";
                right->kppArray[0].record = nullptr;

                //shift rightnode cells 1 to the left
                for (int i = 1; i<getNumKeys(right); i++){
                    right->kppArray[i-1] = right->kppArray[i];
                }

            }

            //update pointer to next node
            targetNode->nextNode = right->nextNode;
            removeInternalNode(parentNode->keyArray[prevIndex],parentNode, right);
        }

    }
       /*
    //index = std::lower_bound(targetNode->kppArray->key.begin(), targetNode->kppArray->key.end(), key) - targetNode->kppArray->key.begin();
    for (KeyPointerPair targetnodePair : targetNode->kppArray){
        int index2 = 0;
        if (key == targetnodePair.key){
            targetNode->kppArray->key.erase(targetNode->kppArray->key.begin() + index);
        }
    }
    */

    //1.Keys in leaf node > Min size
    //KeyPointerPair targetnodePair = targetNode->kppArray[1];
    //NonLeafNode* nonLeafNode = dynamic_cast<NonLeafNode*>(curNode);
    //curNode = dynamic_cast<LeafNode*>(targetNode);


 
}

//get num keys in leaf node
int getNumKeys(LeafNode* node){
    int count = 0;
    for (int i=0; i< n; i++){
        if(node->kppArray[i].key != ""){
            count++;
        }
    }
    return count;
}

int getNumKeysNL(NonLeafNode* node){
    int count = 0;
    for (int i =0;i < n;i++){
        if(node->keyArray[i] != ""){
            count ++;
        }
    }
    return count;
}

//remove internal nodes in tree.
void BPTree::removeInternalNode(string key,NonLeafNode *parent,Node *child){
    //child node is node to be deleted

    //if parent node is root
    if (parent == root){
        
        //if parent only has 1 key, set root to child node
        if(getNumKeysNL(parent) == 1){

            if (parent->ptrArray[0] == child){
                root = parent->ptrArray[1];
            }

            else{
                root = parent->ptrArray[0];

            }
            return;
        }
    }

    //delete key and pointer in parent node

    //find index of key in parent node
    int firstindex = 0;
    for (int i=0; i<n;i++){
        if (parent->keyArray[i] == key){
            int firstindex = i;
            break;
        }
    }
    parent->keyArray[firstindex] = "";
    
    //find index in ptr array to node to be deleted
    int pointerIndex = 0;
    for(int i = 0; i < n+1; i){
        if (parent->ptrArray[i] == child){
            pointerIndex = i;
            break;
        }
    }
    parent->ptrArray[pointerIndex] = nullptr;

    //shift parent nodes to left
    for (int i = firstindex; i < getNumKeysNL(parent); i++){
        parent->keyArray[i] = parent->keyArray[i+1];
        parent->ptrArray[i] = parent->ptrArray[i+1];
    }
    //delete duplicate key and pointer at  last element
    parent->keyArray[getNumKeysNL(parent)-1] = "";
    parent->ptrArray[getNumKeysNL(parent)] = nullptr;

    //If after deletion, parent node > min keys, return
    if (getNumKeysNL(parent) >= n/2)
        return;


    //else find left and right neighbors

}

//update parent node key with key of child node
void BPTree::updateParentKey(int prevIndex,Node *parent,Node *child,std::vector<NonLeafNode*> &path, std::vector<int> &pathIndexes){
    
    while(parent != nullptr){
        //if prev index = 0 (deleted key is leftmost),move up the tree till not = 0
        if(prevIndex == 0){
            parent = path.back();
            prevIndex = pathIndexes.back();

            path.pop_back();
            pathIndexes.pop_back();
        }

        else{
            //update parent node key
            NonLeafNode* parent = dynamic_cast<NonLeafNode*>(parent);
            LeafNode* child = dynamic_cast<LeafNode*>(child);
            parent->keyArray[prevIndex - 1] = child->kppArray[0].key;
            break;
        }

    }

}