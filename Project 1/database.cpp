#include "database.h"
#include <stdexcept>
#include <iostream>

Database::Database(uint databaseSize) : diskManager(databaseSize)
{
    this->bptree = BPTree();
}

Database::~Database() {}

void Database::insertRecord(const Record &record)
{
    try
    {

        int blockId;
        if (freeBlockSlotHash.empty())
        {
            blockId = diskManager.createBlock();
            freeBlockSlotHash[blockId] = diskManager.BLOCK_SIZE / sizeof(Record) - 1; // -1 to account for the record being inserted
        }
        else
        {
            blockId = freeBlockSlotHash.begin()->first; // Get the first block with free slots, retrieve blockId
            freeBlockSlotHash[blockId] -= 1;
            if (freeBlockSlotHash[blockId] == 0) // If the block is full, remove it from the hash
            {
                freeBlockSlotHash.erase(blockId);
            }
        }

        std::shared_ptr<Block> block = diskManager.readBlock(blockId); // Read the block to insert the record into

        int blockOffset = block->getFreeIndex(); // Get the first free index slot in the block
        if (blockOffset == -1)
        {
            throw std::runtime_error("Block is full");
            // insertRecord(key, record);
        }
        else
        {
            block->insertRecord(record, blockOffset);
            diskManager.writeBlock(blockId, block);
            std::string numvotes = std::to_string(record.getNumVotes());
            // bptree.insertKey(key, blockId, blockOffset);
        }
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

// bool Database::deleteRecord(const std::string &keyValue)
// {
//     // Incomplete
//     // bool Database::deleteRecord(const std::string &key)
//     // deal with multiple records with the same keyValue?

//     // Use the B+ tree to find the block ID and offset, then delete the record from the block
//     // Placeholder implementation

//     std::tuple<int, int> result = bptree.search(key);
//     int blockId = std::get<0>(result);
//     int blockOffset = std::get<1>(result);
//     if (blockId >= 0)
//     {
//         std::shared_ptr<Block> block = diskManager.readBlock(blockId);
//         if (blockOffset != -1)
//         {
//             block->deleteRecord(blockOffset);
//             diskManager.writeBlock(blockId, block);
//             // bptree.deleteKey(key);
//             return true;
//         }
//         else
//         {
//             throw std::runtime_error("Invalid BlockOffse");
//         }
//     }
//     else
//     {
//         throw std::runtime_error("Invalid BlockId");
//     }
//     return false;
// }

// std::vector<Record> Database::retrieveRecordByBPTree(std::string attributeValue)
// {
//     std::vector<Record> records;
//     std::vector<std::tuple<int, int>> blockOffsets = bptree.search(attributeValue);
//     for (auto &blockOffset : blockOffsets)
//     {
//         int blockId = std::get<0>(blockOffset);
//         int offset = std::get<1>(blockOffset);
//         std::shared_ptr<Block> block = diskManager.readBlock(blockId);
//         Record record = block->getRecord(offset);
//         records.push_back(record);
//     }
//     return records;
// }

std::vector<Record> Database::retrieveRecordByLinearScan(std::string attributeValue)
{
    std::vector<int> blockIds = diskManager.getAllBlockIds();
    std::vector<Record> queryResult;
    int timeTaken = 0;
    for (auto &blockId : blockIds)
    {
        std::shared_ptr<Block> block = diskManager.readBlock(blockId);
        int blockAccessTime = diskManager.simulateBlockAccessTime(blockId);
        std::vector<Record> blockRecords = block->retrieveAllRecords();
        for (auto &record : blockRecords)
        {
            if (record.getNumVotes() == std::stoi(attributeValue))
            {
                queryResult.push_back(record);
            }
        }
        timeTaken += blockAccessTime;
    }
    std::cout << "Number of blocks accessed: " << blockIds.size() << std::endl;
    std::cout << "Time taken for linear scan: " << timeTaken << "ms" << std::endl;
    return queryResult;
}

// std::vector<Record> Database::retrieveRangeRecordsByBPTree(std::string start, std::string end)

std::vector<Record> Database::retrieveRangeRecordsByLinearScan(std::string start, std::string end)
{
    // Assuming numerical
    int startValue = std::stoi(start);
    int endValue = std::stoi(end);
    std::vector<int> blockIds = diskManager.getAllBlockIds();
    std::vector<Record> queryResult;
    int timeTaken = 0;

    for (auto &blockId : blockIds)
    {
        std::shared_ptr<Block> block = diskManager.readBlock(blockId);
        int blockAccessTime = diskManager.simulateBlockAccessTime(blockId);
        std::vector<Record> blockRecords = block->retrieveAllRecords();
        for (auto &record : blockRecords)
        {
            if (record.getNumVotes() >= startValue && record.getNumVotes() <= endValue)
            {
                queryResult.push_back(record);
            }
        }
        timeTaken += blockAccessTime;
    }
    std::cout << "Number of blocks accessed: " << blockIds.size() << std::endl;
    std::cout << "Time taken for linear scan: " << timeTaken << "ms" << std::endl;
    return queryResult;
}

// void Database::deleteRecordByBPTree(std::string attributeValue)

void Database::deleteRecordsByLinearScan(std::string attributeValue)
{
    std::vector<int> blockIds = diskManager.getAllBlockIds();
    int timeTaken = 0;
    for (auto &blockId : blockIds)
    {
        std::shared_ptr<Block> block = diskManager.readBlock(blockId);
        int blockAccessTime = diskManager.simulateBlockAccessTime(blockId);
        std::vector<Record> blockRecords = block->retrieveAllRecords();
        for (int i = 0; i < blockRecords.size(); i++)
        {
            if (blockRecords[i].getNumVotes() == std::stoi(attributeValue))
            {
                block->deleteRecord(i);
            }
        }
        timeTaken += blockAccessTime;
    }
    std::cout << "Number of blocks accessed: " << blockIds.size() << std::endl;
    std::cout << "Time taken for linear scan: " << timeTaken << "ms" << std::endl;
}