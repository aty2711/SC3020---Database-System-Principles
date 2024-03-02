#include "tree_helper.h"

/*
~~~~~~~~~~~~~~~~~~~~~~~ KeyPointerPair ~~~~~~~~~~~~~~~~~~~~~~~~
*/

// Default constructor
KeyPointerPair::KeyPointerPair() : key(nulldouble), record(nullptr) {}

// Constructor initializing all attributes
KeyPointerPair::KeyPointerPair(double key, string* record) : key(key), record(record) {}

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
        keyArray[i] = nulldouble;
    }
    for (int i = 0; i < n + 1; i++) {
        ptrArray[i] = nullptr;
    }
}

// Constructor initializing all attributes
NonLeafNode::NonLeafNode(Node* initialPtrArray[n + 1], double initialKeyArray[n]) {
    // Copy the values from argument to attribute
    for (int i = 0; i < n + 1; ++i) {
        ptrArray[i] = initialPtrArray[i];
    }
    for (int i = 0; i < n; ++i) {
        keyArray[i] = initialKeyArray[i];
    }
}