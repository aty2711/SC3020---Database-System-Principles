
/**
 * @file database.h
 * @brief Defines the Database class for managing database operations and disk storage.
 *
 * Database class is a high-level interface between the data through the disk manager and the B+ tree.
 * It provides a simplified model of database operations, including inserting, searching, deleting records,
 * and retrieving range of records. It also provides a simplified model of disk operations, including simulating
 * block read and write operations, block allocation and deallocation, and disk space management.
 */

#ifndef DATABASE_H
#define DATABASE_H

#include "block.h"
#include "disk_manager.h"
#include "b_plus_tree.h"

#include <memory>
#include <unordered_map>

typedef unsigned int uint;
typedef unsigned char uchar;

class Database
{
private:
    DiskManager diskManager;                        // Simulate disk storage operations such as reading blocks, writing blocks
    BPTree bptree;                                  // Simulate B+ tree operations such as inserting, searching, deleting records, merging nodes, splitting nodes
    std::unordered_map<int, int> freeBlockSlotHash; // Map block ID to the number of free slots in the block

    int getFreeBlock();
    void incrementFreeBlock(int blockId);

public:
    Database(uint databaseSize);
    ~Database();

    BPTree getBPTree() const { return bptree; };
    DiskManager getDiskManager() const { return diskManager; };

    void insertRecord(const Record &record);
    void deleteRecordByBPTree(std::string attributeValue);
    void deleteRecordsByLinearScan(std::string attributeValue);
    std::vector<Record> retrieveRecordByBPTree(std::string attributeValue);
    std::vector<Record> retrieveRecordByLinearScan(std::string attributeValue);
    std::vector<Record> retrieveRangeRecordsByBPTree(std::string start, std::string end);
    std::vector<Record> retrieveRangeRecordsByLinearScan(std::string start, std::string end);
};

#endif // DATABASE_H
