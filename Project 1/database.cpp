#include "database.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <iomanip>

Database::Database(uint databaseSize) : diskManager(databaseSize)
{
    this->bptree = BPTree();
}

Database::~Database() {}

/**
 * @brief Find a free slot in Blocks, if not found, create a new block. Consumes 1 record slot in the freeBlockSlotHash.
 *
 * @return blockId
 */
int Database::getFreeBlock()
{
    int blockId;
    if (freeBlockSlotHash.empty())
    {
        blockId = diskManager.createBlock();
        freeBlockSlotHash[blockId] = diskManager.BLOCK_SIZE / sizeof(Record) - 1; // -1 to account for the record being inserted
    }
    else
    {
        blockId = freeBlockSlotHash.begin()->first;
        // Consume a free slot
        freeBlockSlotHash[blockId] -= 1;
        if (freeBlockSlotHash[blockId] == 0)
        {
            freeBlockSlotHash.erase(blockId);
        }
    }
    return blockId;
}

void Database::incrementFreeBlock(int blockId)
{
    // search for the blockId in the freeBlockSlotHash,
    // if not found (means it was full), add it to hash with 1 slot free
    // else increment the free slot count
    if (freeBlockSlotHash.find(blockId) == freeBlockSlotHash.end())
    {
        freeBlockSlotHash[blockId] = 1;
    }
    else
    {
        freeBlockSlotHash[blockId] += 1;
    }
}

void Database::insertRecord(const Record &record)
{
    try
    {
        int blockId = getFreeBlock();

        // Insert the record into the block
        Block block = diskManager.readBlock(blockId); // Read the block to insert the record into.
        int blockOffset = block.getFreeIndex();       // Get the first free index slot in the block
        if (blockOffset == -1)
        {
            throw std::runtime_error("Block is full");
            // insertRecord(key, record);
        }
        else
        {
            block.insertRecord(record, blockOffset);
            diskManager.writeBlock(blockId, block);
            std::string numvotes = std::to_string(record.getNumVotes());
            bptree.insertKey(record.getNumVotes(), blockId, blockOffset);
        }
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Database::deleteRecordByBPTree(int attributeValue)
{
    double timeTaken = 0;
    std::vector<std::tuple<int, int>> recordAddresses = bptree.exactSearch(attributeValue);
    for (auto &recordAddress : recordAddresses)
    {
        int blockId = std::get<0>(recordAddress); // depending on what is the return of bptree.search
        int offset = std::get<1>(recordAddress);
        Block block = diskManager.readBlock(blockId);
        timeTaken += diskManager.simulateBlockAccessTime(blockId);
        block.deleteRecord(offset);
        diskManager.writeBlock(blockId, block);
        timeTaken += diskManager.simulateBlockAccessTime(blockId);
        incrementFreeBlock(blockId);
        bptree.deleteKey(attributeValue);
    }
}

void Database::deleteRecordsByLinearScan(int attributeValue)
{
    std::vector<int> blockIds = diskManager.getAllBlockIds();
    int timeTaken = 0;
    for (auto &blockId : blockIds)
    {
        Block block = diskManager.readBlock(blockId);
        timeTaken += diskManager.simulateBlockAccessTime(blockId);

        // Retrieve all records in the block and delete the ones with the attribute value
        std::vector<Record> blockRecords = block.retrieveAllRecords();
        for (int i = 0; i < blockRecords.size(); i++)
        {
            if (blockRecords[i].getNumVotes() == attributeValue)
            {
                block.deleteRecord(i);
                incrementFreeBlock(blockId);
            }
        }
        diskManager.writeBlock(blockId, block);
        timeTaken = diskManager.simulateBlockAccessTime(blockId);
    }
    std::cout << "Number of blocks accessed: " << blockIds.size() << std::endl;
    std::cout << "Time taken for linear scan: " << timeTaken << "ms" << std::endl;
}

std::vector<Record> Database::retrieveRecordByBPTree(int attributeValue)
{
    double timeTaken = 0;
    int recordCount = 0;
    double totalAverageRating = 0;
    std::vector<Record> records;
    std::vector<std::tuple<int, int>> recordAddresses = bptree.exactSearch(attributeValue);
    for (auto &recordAddress : recordAddresses)
    {
        int blockId = std::get<0>(recordAddress);
        int offset = std::get<1>(recordAddress);
        Block block = diskManager.readBlock(blockId);
        Record record = block.retrieveRecord(offset);
        records.push_back(record);
        recordCount++;
        totalAverageRating += record.getAverageRating();
        timeTaken += diskManager.simulateBlockAccessTime(blockId);
    }

    double averageOfAverageRating = totalAverageRating / recordCount;

    std::cout << "Number of blocks accessed: " << recordAddresses.size() << std::endl;
    std::cout << "Number of records: " << recordCount << std::endl;
    std::cout << "Time taken for bpt scan: " << timeTaken << "ms" << std::endl;
    std::cout << "Average rating: " << std::fixed << std::setprecision(4) << averageOfAverageRating << std::endl;
    return records;
}

std::vector<Record> Database::retrieveRecordByLinearScan(int attributeValue)
{
    std::vector<int> blockIds = diskManager.getAllBlockIds();
    std::vector<Record> queryResult;
    double timeTaken = 0;
    int recordCount = 0;
    double totalAverageRating = 0;
    for (auto &blockId : blockIds)
    {
        Block block = diskManager.readBlock(blockId);
        timeTaken += diskManager.simulateBlockAccessTime(blockId);
        std::vector<Record> blockRecords = block.retrieveAllRecords();
        for (auto &record : blockRecords)
        {
            if (record.getNumVotes() == attributeValue)
            {
                queryResult.push_back(record);
                recordCount++;
                totalAverageRating += record.getAverageRating();
            }
        }
    }
    double averageOfAverageRating = totalAverageRating / recordCount;
    std::cout << "Number of blocks accessed: " << blockIds.size() << std::endl;
    std::cout << "Number of records: " << recordCount << std::endl;
    std::cout << "Time taken for linear scan: " << timeTaken << "ms" << std::endl;
    std::cout << "Average rating: " << std::fixed << std::setprecision(4) << averageOfAverageRating << std::endl;

    std::sort(queryResult.begin(), queryResult.end(), [](const Record &a, const Record &b)
              { return a.getTconst() < b.getTconst(); });

    return queryResult;
}

std::vector<Record> Database::retrieveRangeRecordsByBPTree(int start, int end)
{
    double timeTaken = 0;
    std::vector<Record> records;
    int recordCount = 0;
    double totalAverageRating = 0;
    std::vector<std::tuple<int, int>> recordAddresses = bptree.rangeSearch(start, end);
    for (auto &recordAddress : recordAddresses)
    {
        int blockId = std::get<0>(recordAddress);
        int offset = std::get<1>(recordAddress);
        Block block = diskManager.readBlock(blockId);
        Record record = block.retrieveRecord(offset);
        records.push_back(record);
        recordCount++;
        totalAverageRating += record.getAverageRating();
        timeTaken += diskManager.simulateBlockAccessTime(blockId);
    }
    double averageOfAverageRating = totalAverageRating / recordCount;
    std::cout << "Number of blocks accessed: " << recordAddresses.size() << std::endl;
    std::cout << "Number of records: " << recordCount << std::endl;
    std::cout << "Time taken for bpt scan: " << timeTaken << "ms" << std::endl;
    std::cout << "Average rating: " << std::fixed << std::setprecision(4) << averageOfAverageRating << std::endl;
    return records;
}

std::vector<Record> Database::retrieveRangeRecordsByLinearScan(int start, int end)
{
    // Assuming numerical
    std::vector<int> blockIds = diskManager.getAllBlockIds();
    std::vector<Record> queryResult;
    double timeTaken = 0;
    int recordCount = 0;
    double totalAverageRating = 0;

    for (auto &blockId : blockIds)
    {
        // std::shared_ptr<Block> block = diskManager.readBlock(blockId);
        Block block = diskManager.readBlock(blockId);
        timeTaken += diskManager.simulateBlockAccessTime(blockId);
        std::vector<Record> blockRecords = block.retrieveAllRecords();

        for (auto &record : blockRecords)
        {
            if (record.getNumVotes() >= start && record.getNumVotes() <= end)
            {
                queryResult.push_back(record);
                recordCount++;
                totalAverageRating += record.getAverageRating();
            }
        }
    }

    double averageOfAverageRating = totalAverageRating / recordCount;

    std::cout << "Number of blocks accessed: " << blockIds.size() << std::endl;
    std::cout << "Number of records: " << recordCount << std::endl;
    std::cout << "Time taken for linear scan: " << timeTaken << "ms" << std::endl;
    std::cout << "Average rating: " << std::fixed << std::setprecision(4) << averageOfAverageRating << std::endl;

    std::sort(queryResult.begin(), queryResult.end(), [](const Record &a, const Record &b)
              { return a.getTconst() < b.getTconst(); });

    return queryResult;
}
