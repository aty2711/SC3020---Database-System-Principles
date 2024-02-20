#include "block.h"
#include <iostream>

bool Block::addRecord(const Record &record)
{
    std::string serializedRecord = record.serialize();

    size_t currentSize = 0;
    for (const auto &rec : records)
    {
        currentSize += rec.size() + 1; // +1 for newline or other delimiter
    }
    if (currentSize + serializedRecord.size() + 1 <= MAX_SIZE)
    {
        records.push_back(serializedRecord);
        return true;
    }
    return false;
}

void Block::printRecords() const
{
    for (const auto &record : records)
    {
        std::cout << record << std::endl;
    }
}
