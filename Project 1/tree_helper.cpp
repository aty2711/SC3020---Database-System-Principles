#include "tree_helper.h"

/*
~~~~~~~~~~~~~~~~~~~~~~~ KeyPointerPair ~~~~~~~~~~~~~~~~~~~~~~~~
*/

// Default constructor
KeyPointerPair::KeyPointerPair() : key(nullInt), blockId(nullInt), blockOffset(nullInt) {}

// Constructor initializing all attributes
KeyPointerPair::KeyPointerPair(int key, int blockId, int blockOffset) : key(key), blockId(blockId), blockOffset(blockOffset) {}

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

/*
~~~~~~~~~~~~~~~~~~~~~~~ NonLeafNode ~~~~~~~~~~~~~~~~~~~~~~~~
*/

// Default constructor
NonLeafNode::NonLeafNode() {
    for (int i = 0; i < n; i++) {
        keyArray[i] = nullInt;
    }
    for (int i = 0; i < n + 1; i++) {
        ptrArray[i] = nullptr;
    }
}