#ifndef BLOCK_H
#define BLOCK_H

#include "record.h"
#include <vector>
#include <string>

class Block
{
public:
    std::vector<std::string> records;
    static const int MAX_SIZE = 200; // Block size in bytes

    bool addRecord(const Record &record);
    void printRecords() const;
};

#endif // BLOCK_H
