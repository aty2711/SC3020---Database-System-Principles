#include <iostream>
#include <stack>
#include <cmath>
#include <tuple>
#include "b_plus_tree.h"
#include "tree_helper.h"
using namespace std;

int BPTree::getTreeHeight()
{
    if (root == nullptr)
    {
        // Empty tree
        return 0;
    }
    else
    {
        // Recursively fetch one level below until leaf node is reached

        int height = 1;
        Node *cur = root;

        // Check if cur is a LeafNode. If not, check downwards
        NonLeafNode *nonLeafNode = dynamic_cast<NonLeafNode *>(cur);
        while (nonLeafNode != nullptr)
        {
            cur = nonLeafNode->ptrArray[0]; // First pointer of node
            nonLeafNode = dynamic_cast<NonLeafNode *>(cur);
            height++;
        }

        return height;
    }
}

vector<tuple<int, int>> BPTree::exactSearch(int key)
{
    Node *cur = getLeafNode(key, false);

    // For each exact match, store the resulting pointer
    vector<tuple<int, int>> results;
    LeafNode *leafNode = dynamic_cast<LeafNode *>(cur);
    bool isSearching = true; // keep track of whether the next LeafNode still needs to be searched or not

    // Continue looping until reached last LeafNode or key is greater than target
    while (isSearching || leafNode != nullptr)
    {
        // Loop through one LeafNode
        for (KeyPointerPair kpp : leafNode->kppArray)
        {

            // Check if exact match
            if (key == kpp.key)
            {
                tuple<int, int> recordPtr = make_tuple(kpp.blockId, kpp.blockOffset);
                results.push_back(recordPtr);
            }
            else if (key < kpp.key)
            {

                // The rest of the keys are greater than the target
                // No need to search anymore
                isSearching = false;
                return results;
            }
            // Otherwise, key is smaller than target, or key is empty.
            // Continue searching the next LeafNode
        }

        // Fetch next LeafNode in linked list to continue searching
        cur = leafNode->nextNode;
        leafNode = dynamic_cast<LeafNode *>(cur);
    }

    return results;
}

vector<tuple<int, int>> BPTree::rangeSearch(int low, int high)
{
    Node *cur = getLeafNode(low, false);

    // For each exact match, store the resulting pointer
    vector<tuple<int, int>> results;
    LeafNode *leafNode = dynamic_cast<LeafNode *>(cur);
    bool isSearching = true; // keep track of whether the next LeafNode still needs to be searched or not

    // Continue looping until reached last LeafNode or key is greater than upper bound
    while (isSearching || leafNode != nullptr)
    {
        // Loop through one LeafNode
        for (KeyPointerPair kpp : leafNode->kppArray)
        {

            // Check if within range
            if (low <= kpp.key && high >= kpp.key)
            {
                tuple<int, int> recordPtr = make_tuple(kpp.blockId, kpp.blockOffset);
                results.push_back(recordPtr);
            }
            else if (high < kpp.key)
            {

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
        leafNode = dynamic_cast<LeafNode *>(cur);
    }

    return results;
}

int BPTree::getNumIndexNodes(int key)
{
    Node *cur = root;
    int num = 0;

    // Check if cur is a LeafNode. If not, check downwards
    NonLeafNode *nonLeafNode = dynamic_cast<NonLeafNode *>(cur);
    while (nonLeafNode != nullptr)
    {
        // This node is a non-leaf node, increment answer by 1
        num++;

        // Determine the next node to go downwards
        int index = 0;
        for (double i : nonLeafNode->keyArray)
        {
            if (key > i && i != nullInt)
            {
                index++;
            }
            else
            {
                break;
            }
        }
        cur = nonLeafNode->ptrArray[index];

        // Check while loop condition
        nonLeafNode = dynamic_cast<NonLeafNode *>(cur);
    }

    return num;
}

int BPTree::getTotalNumNodes()
{
    // Use DFS to traverse through all nodes
    int numNodes = 0;
    stack<Node *> nodeStack;
    nodeStack.push(root); // Start with the root node

    // Begin DFS
    Node *cur;
    while (!nodeStack.empty())
    {
        // Pop a value from the stack
        cur = nodeStack.top();
        nodeStack.pop();
        numNodes++;

        // Process the current Node
        // Check if this is a LeafNode
        LeafNode *leafNode = dynamic_cast<LeafNode *>(cur);
        if (leafNode != nullptr)
        {
            // Nothing more needs to be done, continue the while loop
            continue;
        }

        // Otherwise, this should be a NonLeafNode
        NonLeafNode *nonLeafNode = dynamic_cast<NonLeafNode *>(cur);
        if (nonLeafNode != nullptr)
        {
            // Add all Node pointers to the stack
            for (Node *ptr : nonLeafNode->ptrArray)
            {
                if (ptr != nullptr)
                {
                    nodeStack.push(ptr);
                }
            }

            // // For debugging purposes
            // // Dump all of the keys in this node

            // // Traverse through one whole Node
            // int keysOfNode[n];
            // int key;
            // for (int i = 0; i < n; i++) {
            //     key = nonLeafNode->keyArray[i];
            //     if (key != nullInt) {
            //         // Add all non-null keys into temporary string array
            //         keysOfNode[i] = key;
            //     } else {
            //         keysOfNode[i] = nullInt;
            //     }
            // }

            // // Print these keys out
            // cout << "(";
            // int length = sizeof(keysOfNode)/sizeof(keysOfNode[0]);
            // for (int i = 0; i < length; i++) {
            //     if (keysOfNode[i] != nullInt) {
            //         cout << keysOfNode[i];
            //     }

            //     // Print a comma
            //     if (i != length - 1) {
            //         cout << ",";
            //     }
            // }
            // cout << ") & ";
        }
    }

    return numNodes;
}

void BPTree::displayLeafNodes()
{
    Node *cur = getLeafNode(nullInt, false); // go to leftmost LeafNode directly

    LeafNode *leafNode = dynamic_cast<LeafNode *>(cur);
    do
    {
        int keysOfNode[n]; // keys for this node

        // Traverse through one whole Node
        for (int i = 0; i < n; i++)
        {
            int key = leafNode->kppArray[i].key;
            if (key != nullInt)
            {
                // Add all non-null keys into temporary string array
                keysOfNode[i] = key;
            }
            else
            {
                keysOfNode[i] = nullInt;
            }
        }

        // Print these keys out
        cout << "(";
        int length = sizeof(keysOfNode) / sizeof(keysOfNode[0]);
        for (int i = 0; i < length; i++)
        {
            if (keysOfNode[i] != nullInt)
            {
                cout << keysOfNode[i];
            }

            // Print a comma
            if (i != length - 1)
            {
                cout << ",";
            }
        }
        cout << ") -> ";

        // Traverse to next LeafNode in linked list
        cur = leafNode->nextNode;
        leafNode = dynamic_cast<LeafNode *>(cur);

    } while (cur != nullptr);
}

void BPTree::displayRootNode()
{
    NonLeafNode* nonLeafNode = dynamic_cast<NonLeafNode*>(root);
    int keysOfNode[n]; // keys for this node

    // Traverse through one whole Node
    for (int i = 0; i < n; i++)
    {
        int key = nonLeafNode->keyArray[i];
        if (key != nullInt)
        {
            // Add all non-null keys into temporary string array
            keysOfNode[i] = key;
        }
        else
        {
            keysOfNode[i] = nullInt;
        }
    }

    // Print these keys out
    cout << "(";
    int length = sizeof(keysOfNode) / sizeof(keysOfNode[0]);
    for (int i = 0; i < length; i++)
    {
        if (keysOfNode[i] != nullInt)
        {
            cout << keysOfNode[i];
        }

        // Print a comma
        if (i != length - 1)
        {
            cout << ",";
        }
    }
    cout << ")";
}

void BPTree::insertKey(int key, int blockId, int blockOffset)
{
    // If the B+ tree is empty, create a new LeafNode and insert there
    if (root == nullptr)
    {
        // Create new LeafNode
        LeafNode *newLeafNode = new LeafNode();
        newLeafNode->kppArray[0].key = key;
        newLeafNode->kppArray[0].blockId = blockId;
        newLeafNode->kppArray[0].blockOffset = blockOffset;

        // Assign root to new LeafNode
        root = newLeafNode;

        return;
    }

    // Check where to insert the key
    LeafNode *targetNode = dynamic_cast<LeafNode *>(getLeafNode(key, true));

    // Check whether the target node is already full
    bool isFull = true;
    for (KeyPointerPair kpp : targetNode->kppArray)
    {
        // isFull will be set to false if at least one key is missing
        if (kpp.key == nullInt)
        {
            isFull = false;
            break;
        }
    }

    if (isFull)
    {
        // If LeafNode is full, then need to involve parent node in insertion

        // Create a sorted temporary list of KeyPointerPairs
        KeyPointerPair tempKpps[n + 1];
        int tempKppsIndex = 0;
        bool isInserted = false; // Check if new key is already inserted
        for (KeyPointerPair kpp : targetNode->kppArray)
        {
            if (key < kpp.key && !isInserted)
            {
                KeyPointerPair newKpp = KeyPointerPair(key, blockId, blockOffset);
                tempKpps[tempKppsIndex++] = newKpp;
                isInserted = true;
            }
            tempKpps[tempKppsIndex++] = kpp;
        }
        if (!isInserted)
        {
            // The new key is bigger than all other keys
            KeyPointerPair newKpp = KeyPointerPair(key, blockId, blockOffset);
            tempKpps[tempKppsIndex++] = newKpp;
            isInserted = true;
        }

        // Determine the middle element of the temp list
        // Index is half rounded down
        // Middle element will be present in parent node
        int middleIndex = floor((n + 1) / 2);
        KeyPointerPair middleKpp = tempKpps[middleIndex];

        // Split the LeafNode into two LeafNodes
        LeafNode *newLeafNode = new LeafNode();
        int nodeIndex = 0;
        for (int i = middleIndex; i < n + 1; i++)
        {
            // Insert middle element onwards to new LeafNode
            newLeafNode->kppArray[nodeIndex].key = tempKpps[i].key;
            newLeafNode->kppArray[nodeIndex].blockId = tempKpps[i].blockId;
            newLeafNode->kppArray[nodeIndex].blockOffset = tempKpps[i].blockOffset;
            nodeIndex++;
        }
        for (int i = 0; i < n; i++)
        {
            // Empty the target node
            targetNode->kppArray[i].key = nullInt;
            targetNode->kppArray[i].blockId = nullInt;
            targetNode->kppArray[i].blockOffset = nullInt;
        }
        for (int i = 0; i < middleIndex; i++)
        {
            // Rewrite the elements in the target LeafNode
            targetNode->kppArray[i].key = tempKpps[i].key;
            targetNode->kppArray[i].blockId = tempKpps[i].blockId;
            targetNode->kppArray[i].blockOffset = tempKpps[i].blockOffset;
        }

        // Reassign pointer of the target LeafNode and new LeafNode
        newLeafNode->nextNode = targetNode->nextNode;
        targetNode->nextNode = newLeafNode;

        // Determine the parent of the target node
        vector<NonLeafNode *> nodePath = getNodePath(middleKpp);
        if (nodePath.size() == 0)
        {
            // Node currently has no parent

            // Find the attributes required to create a new
            // NonLeafNode instance as parent node
            NonLeafNode *parentNode = new NonLeafNode();
            parentNode->ptrArray[0] = targetNode;
            parentNode->keyArray[0] = middleKpp.key;

            // The pointer after the key in the parent node
            // Should point to the newly created LeafNode
            parentNode->ptrArray[1] = newLeafNode;

            // Set parent node as root node
            root = parentNode;
        }
        else
        {
            // Node has non-empty parent
            // Insert new key into parent
            insertInternalNode(middleKpp.key, nodePath, newLeafNode);
        }
    }
    else
    {
        // Node is not yet full
        // Insert the key into the right place, and then push all other
        // KeyPointerPairs backwards
        int targetIndex = 0;
        for (KeyPointerPair kpp : targetNode->kppArray)
        {

            // Find the first key greater than the inserting key
            // Or empty key
            if (key < kpp.key || kpp.key == nullInt)
            {
                break;
            }
            targetIndex++;
        }

        // Push all of the KeyPointerPairs back until the targetIndex
        for (int i = n - 2; i >= targetIndex; i--)
        {
            targetNode->kppArray[i + 1] = targetNode->kppArray[i];
        }

        // Insert the KeyPointerPair into the empty slot
        targetNode->kppArray[targetIndex] = KeyPointerPair(key, blockId, blockOffset);
    }
}

Node *BPTree::getLeafNode(int key, bool insert)
{
    Node *cur = root;

    // Check if cur is a LeafNode. If not, check downwards
    NonLeafNode *nonLeafNode = dynamic_cast<NonLeafNode *>(cur);
    while (nonLeafNode != nullptr)
    {

        // Determine the next node to go downwards
        int index = 0;
        for (double i : nonLeafNode->keyArray)
        {
            // If key argument is equal to current key
            bool direction;
            if (insert) 
            {
                // For insert functions, go to rightmost node
                direction = key >= i;
            } else {
                // For search functions, go to leftmost node
                direction = key > i;
            }

            if (direction && i != nullInt)
            {
                index++;
            }
            else
            {
                break;
            }
        }
        cur = nonLeafNode->ptrArray[index];

        // Check while loop condition
        nonLeafNode = dynamic_cast<NonLeafNode *>(cur);
    }

    return cur;
}

vector<NonLeafNode *> BPTree::getNodePath(KeyPointerPair kpp)
{
    vector<NonLeafNode *> nodePath;

    // If the B+ tree has no NonLeafNode layer, return null pointer
    NonLeafNode *nonLeafNode = dynamic_cast<NonLeafNode *>(root);
    if (nonLeafNode == nullptr)
    {
        return nodePath;
    }

    // Check downwards
    Node *cur = root;
    while (nonLeafNode != nullptr)
    {

        // Determine the next node to go downwards
        int index = 0;
        for (double i : nonLeafNode->keyArray)
        {
            if (kpp.key > i && i != nullInt)
            {
                index++;
            }
            else
            {
                break;
            }
        }
        cur = nonLeafNode->ptrArray[index];

        // Add node to path
        nodePath.push_back(nonLeafNode);

        // Check while loop condition
        nonLeafNode = dynamic_cast<NonLeafNode *>(cur);
    }

    return nodePath;
}

void BPTree::insertInternalNode(int key, vector<NonLeafNode *> nodePath, Node *nextPtr)
{
    // Retrieve current NonLeafNode which is right above the previous
    // NonLeafNode that was being inspected
    NonLeafNode *cur = nodePath.back();
    nodePath.pop_back(); // in case of future calls of this function

    // Check whether this node is already full
    bool isFull = true;
    for (int key : cur->keyArray)
    {
        // isFull will be set to false if at least one key is missing
        if (key == nullInt)
        {
            isFull = false;
            break;
        }
    }

    if (isFull)
    {
        // If current node is full, then need to involve parent node in insertion

        // Create a sorted temporary list of keys
        double tempKeys[n + 1];
        int tempKeysIndex = 0;
        int insertPtrIndex = 0;  // Track where the pointer should be inserted
        bool isInserted = false; // Check if new key is already inserted
        for (int curKey : cur->keyArray)
        {
            if (key < curKey && !isInserted)
            {
                tempKeys[tempKeysIndex++] = key;
                insertPtrIndex = tempKeysIndex;
                isInserted = true;
            }
            tempKeys[tempKeysIndex++] = curKey;
        }
        if (!isInserted)
        {
            // The new key is bigger than all other keys
            tempKeys[tempKeysIndex++] = key;
            insertPtrIndex = tempKeysIndex;
            isInserted = true;
        }

        // Create a sorted temporary list of pointers
        Node *tempPtrs[n + 2];
        int tempPtrsIndex = 0;
        isInserted = false;
        for (int i = 0; i < n + 1; i++)
        {
            if (i == insertPtrIndex)
            {
                tempPtrs[tempPtrsIndex++] = nextPtr;
                isInserted = true;
            }
            tempPtrs[tempPtrsIndex++] = cur->ptrArray[i];
        }
        if (!isInserted)
        {
            // Insert pointer at tail end
            tempPtrs[tempPtrsIndex++] = nextPtr;
            isInserted = true;
        }

        // Determine the middle element of the temp list
        // Index is half rounded down
        // Middle element will be present in parent node
        int middleIndex = floor((n + 1) / 2);
        int middleKey = tempKeys[middleIndex];

        // Split the NonLeafNode into two NonLeafNodes
        NonLeafNode *newNonLeafNode = new NonLeafNode();
        int nodeIndex = 0;
        for (int i = middleIndex + 1; i < n + 1; i++)
        {
            // Insert middle element + 1 key onwards to new NonLeafNode
            newNonLeafNode->keyArray[nodeIndex++] = tempKeys[i];
        }
        nodeIndex = 0;
        for (int i = middleIndex + 1; i < n + 2; i++)
        {
            // Insert middle element + 1 pointer onwards to new NonLeafNode
            newNonLeafNode->ptrArray[nodeIndex++] = tempPtrs[i];
        }
        for (int i = 0; i < n; i++)
        {
            // Empty all keys from current node
            cur->keyArray[i] = nullInt;
        }
        for (int i = 0; i < n + 1; i++)
        {
            // Empty all pointers from current node
            cur->ptrArray[i] = nullptr;
        }
        for (int i = 0; i < middleIndex; i++)
        {
            // Rewrite the keys in the current node
            cur->keyArray[i] = tempKeys[i];
        }
        for (int i = 0; i < middleIndex + 1; i++)
        {
            // And also rewrite pointers for current node
            cur->ptrArray[i] = tempPtrs[i];
        }

        // Determine the parent of the target node
        if (nodePath.size() == 0)
        {
            // Node currently has no parent

            // Find the attributes required to create a new
            // NonLeafNode instance as parent node
            NonLeafNode *parentNode = new NonLeafNode();
            parentNode->ptrArray[0] = cur;
            parentNode->keyArray[0] = middleKey;

            // The pointer after the key in the parent node
            // Should point to the newly created NonLeafNode
            parentNode->ptrArray[1] = newNonLeafNode;

            // Set the root to this parent
            root = parentNode;
        }
        else
        {
            // Node has non-empty parent
            // Insert new key into parent
            insertInternalNode(middleKey, nodePath, newNonLeafNode);
        }
    }
    else
    {
        // Node is not yet full
        // Insert the key into the right place, and then push all other
        // keys and pointers backwards
        int targetIndex = 0;
        for (int curKey : cur->keyArray)
        {

            // Find the first key greater than the inserting key
            // Or empty key
            if (key < curKey || curKey == nullInt)
            {
                break;
            }
            targetIndex++;
        }

        // Push all of the pointers back until after the targetIndex
        for (int i = n - 1; i >= targetIndex + 1; i--)
        {
            cur->ptrArray[i + 1] = cur->ptrArray[i];
        }
        // Push all of the keys back until the targetIndex
        for (int i = n - 2; i >= targetIndex; i--)
        {
            cur->keyArray[i + 1] = cur->keyArray[i];
        }

        // Insert the key into the empty slot
        cur->keyArray[targetIndex] = key;
        cur->ptrArray[targetIndex + 1] = nextPtr;
    }
}

void BPTree::deleteKey(int key){
    
    cout << "Deleting " << key <<  " in tree" << endl;
 
    //Check if empty b tree
    if (this->root == nullptr){
        //cout << "Empty tree" << endl;
        return;
    }
    //initially, find all matches of tree
    vector<tuple<int, int>> matches = exactSearch(key);
    int loops = matches.size();
 
    //if no exact matches, return
    if(matches.size() == 0){
 
        cout << "Key not in tree" << endl;
        return;
    }
    return;
 
    int minKeys = (n+1)/2;
    //for(int times = 0; times < matches.size(); times++)
    //while(!matches.empty())
    //loop 42 times for 1000
    while(loops-- > 0){
 
        //cout <<"Loop: " << times << endl;
        Node *curNode = root;
        int index = 0;
        std::vector<NonLeafNode*> path; //vector to store path of traversal
        std::vector<int>pathIndexes;    //vector to store indexes of path of traversal
    
        //find possible target leaf node with key, save the path.
        NonLeafNode* nonLeafNode = dynamic_cast<NonLeafNode*>(curNode);
        while(nonLeafNode != nullptr){
            index = 0;
            for (double i : nonLeafNode->keyArray){
    
                if (key > i && i != nullInt) {
                    index++;
                } 
                else {
                    break;
                }
            }
            
            path.push_back(nonLeafNode);
            pathIndexes.push_back(index);
            curNode = nonLeafNode->ptrArray[index];
            nonLeafNode = dynamic_cast<NonLeafNode*>(curNode);
        }
        //cout << "test "  << endl;
        
 
        //targetnode is leftmost leaf node containing the key
        LeafNode* targetNode = dynamic_cast<LeafNode*>(curNode);
        //find the key's index in leaf node, then delete the records
        //in target node, find same keys and delete the records
        //int firstIndex = std::lower_bound(targetNode->kppArray->key.begin(), targetNode->kppArray->key.end(), key) - targetNode->kppArray->key.begin();
        
        int found = 0;
        int firstIndex = 0;
        //find index of first occurence of key in target node
        while (!found) {
            for (int i = 0; i<n ;i++){
                if (targetNode->kppArray[i].key == key){
                    firstIndex = i;
                    found = 1;
                    break;
                }
            }
            curNode = targetNode->nextNode;
            targetNode = dynamic_cast<LeafNode*>(curNode);
        }
 
        //cout << "first indexes " << firstIndex << endl; 
        //cout << "key at index " << targetNode->kppArray[firstIndex].key << endl;
        //cout << "key at last index " << targetNode->kppArray[getNumKeys(targetNode) - 1].key << endl;
 
        //  Node *target = parent1->ptrArray[index-1];
        //  LeafNode *targetNode = dynamic_cast<LeafNode*>(target);
        //finds the parent node with target nodes then iterate thorugh target nodes to find those
        //containing key to delete
        NonLeafNode *parent1 = path.back();  
        //    LeafNode *targetNode = dynamic_cast<LeafNode*>(parent1->ptrArray[i]);
        bool deleted = false;
 
        for (int j=0 ; j< n ; j++){
            int nodeKey = targetNode->kppArray[j].key;
            if (nodeKey == key ){
                deleted = true;
                int oldNum = getNumKeys(targetNode);
 
                //delete key
                targetNode->kppArray[j].key = nullInt;
                targetNode->kppArray[j].blockId = nullInt;
                targetNode->kppArray[j].blockOffset = nullInt;
 
                cout << "Deleted "<< key <<" at index "<< j << "\n";
 
                //if node deleted is not last , need to shift left and delete duplicate
                if(j != (oldNum -1)){
 
                    //shift keys to the left after every deletion
                    for (int k=j ; k < getNumKeys(targetNode)  ; k++){
                        targetNode->kppArray[k] = targetNode->kppArray[k+1];
                    }
 
                    //clear duplicate last key 
                    targetNode->kppArray[getNumKeys(targetNode) -1].key = nullInt;
                    targetNode->kppArray[getNumKeys(targetNode) -1].blockId = nullInt;
                    targetNode->kppArray[getNumKeys(targetNode) -1].blockOffset = nullInt;
 
 
                }
                break;
 
            }
            if (deleted)
                break;
 
        }
        
        
 
        if (deleted){
            int borrowed = 0;
            int merged = 0;
            //handle node balancing
            cout << "Deleted 1 instance, now handling cases.\n";
            //after deleting only 1 key ,handle case scenarios
            if (getNumKeys(targetNode) >= minKeys){
 
                cout << "target node > min keys " << "\n";
 
                if (firstIndex == 0){
 
                    while(path.back() != nullptr ){
 
                        if (pathIndexes.back() == 0){
                            //path.back()->keyArray[pathIndexes.back()] != key
                            pathIndexes.pop_back();
                            path.pop_back();
                        }
 
                        else{
                            cout << "update parent node key" << "\n";
                            key = targetNode->kppArray[0].key;
 
                            //cout << key << "\n";
                            firstIndex = pathIndexes.back();
                            //cout << firstIndex << "\n";
 
                            Node* parent = path.back();
                            NonLeafNode* parentNode = dynamic_cast<NonLeafNode*>(parent);
 
                            parentNode->keyArray[firstIndex-1] = key;
 
                            //cout << parentNode->keyArray[0] << "\n";
                            //cout << parentNode->keyArray[1] << "\n";
                            //cout << parentNode->keyArray[2] << "\n";
                            //cout << parentNode->keyArray[3] << "\n";
 
 
                            //cout << "done" << "\n";
                            continue;
                        }
                    }
                }
 
            }
 
            //else Keys in leaf node < Min size
            else if (getNumKeys(targetNode) < minKeys){
 
                cout << "target node < min keys " << "\n";
                int prevIndex = pathIndexes.back();
                NonLeafNode *parentNode = path.back();
                //cout << "prevIndex " << prevIndex << "\n";
                Node *leftNode;
                Node *rightNode; //left right pointers to keep track of neighbor
                //path for parent nodes, ppathIndexes for prev indexes
                path.pop_back();
                pathIndexes.pop_back();
                //cout << "prevIndex is  " << prevIndex << endl;
                //cout <<  parentNode->keyArray[0] << endl;
                //cout <<  parentNode->keyArray[1] << endl;
                //cout <<  parentNode->keyArray[2] << endl;
                //cout <<  parentNode->keyArray[3] << endl;
                //if there is a left neighbor of same parent node
 
                if(prevIndex > 0){
                    
                    leftNode = parentNode->ptrArray[prevIndex-1]; //left now points to node left of targetnode
                    LeafNode* left = dynamic_cast<LeafNode*>(leftNode);
                    cout << "last of left neighbor " << left->kppArray[getNumKeys(left)-1].key << endl;
                    
                    //if can borrow from left, borrow
                    if(getNumKeys(left) > minKeys){   
                        // = last key in left node
                        cout << "borrow left " << endl;
                        KeyPointerPair borrowleft = left->kppArray[getNumKeys(left) - 1];
 
                        //shift keys in target node by 1 to the right 
                        for (int i = n-1; i >=0 ; i++){
                            targetNode->kppArray[i+1] = targetNode->kppArray[i];
                        }
 
                        //insert into first element of target node
                        targetNode->kppArray[0] = borrowleft;
        
                        //cout << "borrow left " << "\n";
                        //remove last element of left node, doesnt reduce container size
                        left->kppArray[getNumKeys(left) - 1].key = nullInt;
                        left->kppArray[getNumKeys(left) - 1].blockId = nullInt;
                        left->kppArray[getNumKeys(left) - 1].blockOffset = nullInt;                
 
                        //update key in parent node to new left node added in target node
                        parentNode->keyArray[prevIndex] = targetNode->kppArray[0].key;
                        //return
                        borrowed = 1;
                    }
                }
        
                //check if theres a right neighbor
                if(prevIndex < getNumKeysNL(parentNode) -1 && borrowed == 0){
                    cout << "right neighbor exists " << "\n";
                    rightNode = parentNode->ptrArray[prevIndex+1];
                    LeafNode* right = dynamic_cast<LeafNode*>(rightNode);
                    //cout << "first of right neighbor" << right->kppArray[0].key << endl;
 
                    //if can borrow from right,borrow
                    if(getNumKeys(right) > minKeys){
                        cout << "borrow right " << endl;
                        //first key in right node
                        KeyPointerPair borrowright = right->kppArray[0];
                        //insert into last element of target node
                        targetNode->kppArray[getNumKeys(targetNode)] = borrowright;
 
                        //cout << "borrow right " << "\n"; 
                        //shift keys in right node by 1 to left
                        for (int i = 0 ; i < getNumKeys(right) ; i++){
                            right->kppArray[i] = right->kppArray[i+1];
                        }
 
 
                        //clear duplicate last key 
                        right->kppArray[getNumKeys(right)-1].key = nullInt;
                        right->kppArray[getNumKeys(right)-1].blockId = nullInt;
                        right->kppArray[getNumKeys(right)-1].blockOffset = nullInt;
 
                        //test
                        //cout <<  right->kppArray[0].key << endl;
                        //cout <<  right->kppArray[1].key << endl;
                        //cout <<  right->kppArray[2].key << endl;
                        //cout <<  right->kppArray[3].key << endl;
 
                        //update key in parent node 
                        parentNode->keyArray[prevIndex] = right->kppArray[0].key;
 
                        //test
                        //cout <<  parentNode->keyArray[0]<< endl;
                        //cout <<  parentNode->keyArray[1]<< endl;
                        //cout <<  parentNode->keyArray[2]<< endl;
                        //cout <<  parentNode->keyArray[3]<< endl;
 
        
                        //check if deleted key in target node is the leftmost key , then need update parent key
                        if (firstIndex != 0){
                            updateParentKey(prevIndex,targetNode,parentNode,path,pathIndexes);
                        }
                        borrowed = 1;
        
                    }
        
        
                }
                
                //merging 
                //there is left neighbor with  min keys,  merge 
                if (leftNode != nullptr && prevIndex > 0 && borrowed ==0 && merged ==0){
 
                    cout << "merge left " << "\n";
                    merged = 1;
                    LeafNode*left = dynamic_cast<LeafNode*>(leftNode);
                    //while no. of keys not equal to 0, put all keys from target node and merge into the left
                    while (getNumKeys(targetNode) != 0){
                        //add to last element of left node the first element of target node
                        left->kppArray[getNumKeys(left)] = targetNode->kppArray[0];
 
                        //shift targetnode cells 1 to the left
                        for(int i=0; i<getNumKeys(targetNode); i++){
                            targetNode->kppArray[i] = targetNode->kppArray[i+1];
                        }
        
                        //remove duplicate key ptr in targetnode at last index
                        targetNode->kppArray[getNumKeys(targetNode)].key = nullInt;
                        targetNode->kppArray[getNumKeys(targetNode)].blockId = nullInt;
                        targetNode->kppArray[getNumKeys(targetNode)].blockOffset = nullInt;
                    }
                    //update pointer to next node
                    left->nextNode = targetNode->nextNode;
 
                    //remove internal
                    removeInternalNode(parentNode->keyArray[prevIndex-1], parentNode,targetNode);
                }
        
        
                //else, right neighbor has min keys, and merge
                else if (borrowed == 0 && merged == 0){
 
                    cout << "merge right " << "\n";
                    merged = 1;
                    LeafNode* right = dynamic_cast<LeafNode*>(rightNode);
        
                    while(getNumKeys(right) != 0){
        
                        //add to last element of targetnode the first element of right
                        targetNode->kppArray[getNumKeys(targetNode)] = right->kppArray[0];
 
                        //shift right records 1 to the left
                        for (int i=0 ; i < n -1 ; ++i){
                            right->kppArray[i] = right->kppArray[i+1];
                        }
 
                        //remove duplicate key ptr in rightnode at last index
                        right->kppArray[getNumKeys(right)].key = nullInt;
                        right->kppArray[getNumKeys(right)].blockId = nullInt;
                        right->kppArray[getNumKeys(right)].blockOffset = nullInt;
 
        
                    }
                    /*test
                    cout << targetNode->kppArray[0].key << "\n";
                    cout << targetNode->kppArray[1].key << "\n";
                    cout << targetNode->kppArray[2].key << "\n";
                    cout << targetNode->kppArray[3].key << "\n";
                    
                    
                    cout << right->kppArray[0].key << "\n";
                    cout << right->kppArray[1].key << "\n";
                    cout << right->kppArray[2].key << "\n";
                    cout << right->kppArray[3].key << "\n";
                    */
 
                    //update pointer to next node
                    targetNode->nextNode = right->nextNode;
                    //cout << prevIndex << "\n";
                    removeInternalNode(parentNode->keyArray[prevIndex],parentNode, right);
                }                   
                        
            }
 
                    
        }
 
    }

}

//get num keys in leaf node
int BPTree::getNumKeys(LeafNode* node){
    int count = 0;
    for (int i=0; i< n; i++){
        if(node->kppArray[i].key != nullInt){
            count++;
        }
    }
    return count;
}

int BPTree::getNumKeysNL(NonLeafNode* node){
    int count = 0;
    for (int i =0;i < n;i++){
        if(node->keyArray[i] != nullInt){
            count ++;
        }
    }
    return count;
}

// remove internal nodes in tree.
void BPTree::removeInternalNode(int key,NonLeafNode *parent,Node *child){
    //child node is node to be deleted
    //cout << "remove internal key:" << key << endl;
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
 
    //delete key and pointer in parent node, then shift 1 to the left
    //cout << parent->keyArray[0] << endl;
    //cout << parent->keyArray[1] << endl;
    //cout << parent->keyArray[2] << endl;
    //cout << parent->keyArray[3] << endl;
    //find index of key in parent node

    int keyindex = 0;
    for(int i=0; i<n;i++){
        if (parent->keyArray[i] == key){
            //cout <<"key at at index"<< i << endl;
            keyindex = i;
            break;
        }
    }

    //cout <<"DELETE "<<parent->keyArray[keyindex]<<"at index"<< keyindex << endl;
    parent->keyArray[keyindex] = nullInt;
    //shift keys of parent 1 to the left

    for (int i = keyindex; i < getNumKeysNL(parent)-1 ; i++){
        parent->keyArray[i] = parent->keyArray[i+1];
    }

    //delete duplicate key at last index of parent key array
    parent->keyArray[getNumKeysNL(parent)-1] = nullInt;
    //cout <<"parent size "<< getNumKeysNL(parent) << endl;

    //cout << parent->keyArray[0] << endl;
    //cout << parent->keyArray[1] << endl;
    //cout << parent->keyArray[2] << endl;
    //cout << parent->keyArray[3] << endl;

    //find index in ptr array to node to be deleted
    int pointerIndex = 0;
    for(int i = 0; i < n+1; i++){
        if (parent->ptrArray[i] == child){
            pointerIndex = i;
            break;
        }
    }
    //cout <<"ptr index "<< pointerIndex << endl;
    parent->ptrArray[pointerIndex] = nullptr;
    //shift ptr array to the left 

    for (int i = pointerIndex; i < getNumKeysNL(parent); ++i){
        parent->keyArray[i] = parent->keyArray[i+1];
    }

    //delete duplicate key and pointer at  last element
    parent->ptrArray[getNumKeysNL(parent)] = nullptr;
 


    //If after deletion, parent node > min keys, return
    if (getNumKeysNL(parent) >= n/2){

        //cout << "parent node balanced" << endl;
        return;
    }
 
    //need to borrow from left or right first
    //find left and right neighbors of parent node
    Node *leftParent;
    Node *rightParent;
    Node *ancestor;
    ancestor = findParent(root,parent);

    NonLeafNode* ancestorNode = dynamic_cast<NonLeafNode*>(ancestor);

    //index is index of pointer, from range 0 to numkeys + 1
    int index;
    for (index = 0; index < getNumKeysNL(ancestorNode)+1; index++){
        if (ancestorNode->ptrArray[index] == parent)
            break;
    }
    

    if( index > 0){
        leftParent = parent->ptrArray[index - 1];
        NonLeafNode* leftParent = dynamic_cast<NonLeafNode*>(leftParent);


        //if left neighbor more than min keys
        if ( getNumKeysNL(leftParent) >= (n+1)/2){

            //shift parent nodes 1 to right for key and array seperately
            for (int i= getNumKeysNL(parent)  ; i > 0; i--){
                parent->keyArray[i] = parent->keyArray[i-1];
            }

            for (int i= getNumKeysNL(parent)+1  ; i > 0; i--){
                parent->ptrArray[i] = parent->ptrArray[i-1];
            }

            //insert ptr n key of ancestor into parent index 0
            parent->keyArray[0] = ancestorNode->keyArray[index - 1];
            parent->ptrArray[0] = leftParent->ptrArray[getNumKeysNL(leftParent)];

            //update ancestor key to last key of left neighbor
            ancestorNode->keyArray[index - 1] = leftParent->keyArray[getNumKeysNL(leftParent) - 1];


            //remove last elements of left parent node, not duplicate
            leftParent->keyArray[getNumKeysNL(leftParent)-1] = nullInt;
            leftParent->ptrArray[getNumKeysNL(leftParent)] = nullptr;
            //cout << "borrowed from left neighbor of internal node" << endl;
            return;
        };



    }

    if (index < getNumKeysNL(ancestorNode)+1){
        rightParent = parent->ptrArray[index+1];
        NonLeafNode* rightParent = dynamic_cast<NonLeafNode*>(rightParent);

        //if right  more than min keys
        if(getNumKeysNL(rightParent) > (n+1)/2){
            
            //add ptr and key into parent 
            parent->keyArray[getNumKeysNL(parent)] = ancestorNode->keyArray[index];
            parent->ptrArray[getNumKeysNL(parent)+1] = rightParent->ptrArray[0];
            parent->keyArray[index] = rightParent->keyArray[0];

            //shift right nodes 1 to left for key and ptr seperately

            for (int i=0 ; i < getNumKeysNL(rightParent) ; i++){
                rightParent->keyArray[i] = rightParent->keyArray[i+1];
            }
    
            for (int i=0 ; i < getNumKeysNL(rightParent)+1 ; i++){
                rightParent->ptrArray[i] = rightParent->ptrArray[i+1];
            }
            

            //remove duplicate key ptr of rightnode last index
            rightParent->keyArray[getNumKeysNL(rightParent)] = nullInt;
            rightParent->ptrArray[getNumKeysNL(rightParent)+1] = nullptr;
            //cout << "borrowed from right neighbor of internal node" << endl;
            return;
        }
    }



    //else merge with left or right neighbors
    //merge left 
    if (index > 0){
        //cout << "merge left internal node" << endl;
        leftParent = parent->ptrArray[index - 1];
        NonLeafNode* leftParent = dynamic_cast<NonLeafNode*>(leftParent);

        //add key of ancestor to left neighbor, test if needed
        leftParent->keyArray[getNumKeysNL(leftParent)] = ancestorNode->keyArray[index - 1];
        while (getNumKeysNL(parent) != 0){
            //add pointers and keys from parent to left
            leftParent->keyArray[getNumKeysNL(leftParent)] = parent->keyArray[0];
            leftParent->ptrArray[getNumKeysNL(leftParent)+1] = parent->ptrArray[0];

            //shift parent key left by 1
            for (int i = 0; i < getNumKeysNL(parent); i++){
                parent->keyArray[i] = parent->keyArray[i+1];
            }

            //shift parent pointer left by 1
            for (int i =0; i<getNumKeysNL(parent) + 1;i++){
                parent->ptrArray[i] = parent->ptrArray[i+1];
            }

            //remove duplicate key ptr in parent at last index
            parent->keyArray[getNumKeysNL(parent)] = nullInt;
            parent->ptrArray[getNumKeysNL(parent)+1] = nullptr;
        }
        removeInternalNode(ancestorNode->keyArray[index - 1],ancestorNode,parent);

    }

    //merge right
    else  if (index < getNumKeysNL(ancestorNode)+1) {
        //cout << "merge right internal node" << endl;
        rightParent = parent->ptrArray[index+1];
        NonLeafNode* rightParent = dynamic_cast<NonLeafNode*>(rightParent);

        parent->keyArray[getNumKeysNL(parent)] = ancestorNode->keyArray[index];
        while (getNumKeysNL(rightParent) != 0){

            //add ptr key from right to parent
            parent->keyArray[getNumKeysNL(parent)] = rightParent->keyArray[0];
            parent->ptrArray[getNumKeysNL(parent)+1] = rightParent->ptrArray[0];

            //shift right key left by 1
            for (int i = 0; i < getNumKeysNL(rightParent); i++){
                rightParent->keyArray[i] = rightParent->keyArray[i+1];
            }

            //shift right ptr left by 1
            for (int i = 0; i < getNumKeysNL(rightParent) + 1; i++){
                rightParent->ptrArray[i] = rightParent->ptrArray[i+1];
            }

            //remove duplicate key ptr in right at last index
            rightParent->keyArray[getNumKeysNL(rightParent)] = nullInt;
            rightParent->ptrArray[getNumKeysNL(rightParent)+1] = nullptr;

        }
        removeInternalNode(ancestorNode->keyArray[index],ancestorNode,rightParent);

    }
}
 
// update parent node key with key of child node
void BPTree::updateParentKey(int prevIndex,Node *parent,Node *child,std::vector<NonLeafNode*> &path, std::vector<int> &pathIndexes){
    
    while(path.back() != nullptr){
        //if prev index = 0 (deleted key is leftmost),move up the tree till not = 0
        if(prevIndex == 0){
            path.pop_back();
            pathIndexes.pop_back();
        }
 
        else{
            //update parent node key
            Node* parent = path.back();
            NonLeafNode* parentNode = dynamic_cast<NonLeafNode*>(parent);
            LeafNode* child = dynamic_cast<LeafNode*>(child);
            int p = pathIndexes.back();
            parentNode->keyArray[p-1] = child->kppArray[0].key;
            break;
        }
 
    }
}

//find parent node of node
Node* BPTree::findParent(Node *current, Node *child){
    Node *parent;
    NonLeafNode* currentNode = dynamic_cast<NonLeafNode*>(current);
    //travel to leaf node and get key of first index
    
    if (currentNode != nullptr || currentNode->ptrArray[0] == nullptr){
        return NULL;
    }

    for (int i = 0; i < n+1; i++){
        if (currentNode->ptrArray[i] == child){
            //if ptr in current node points to child, return current node as parent
            parent = currentNode;
            return parent;
        }

        else{
            parent = findParent(currentNode->ptrArray[i],child);

            if (parent != nullptr){
                return parent;
            }
        }

        
    }
    return parent;

}
