#ifndef DATABASE_H
#define DATABASE_H

#include "block.h"
#include "disk_manager.h"
#include "b_plus_tree.h"

#include <memory>
#include <unordered_map>

typedef unsigned int uint;
typedef unsigned char uchar;

/**
 * Database class is a high-level interface between the data through the disk manager and the B+ tree.
 */
class Database
{
private:
    DiskManager diskManager;                        // Simulate disk storage operations such as reading blocks, writing blocks
    BPTree bptree;                                  // Simulate B+ tree operations such as inserting, searching, deleting records, merging nodes, splitting nodes
    std::unordered_map<int, int> freeBlockSlotHash; // Map block ID to the number of free slots in the block

public:
    Database(uint databaseSize);
    ~Database();

    void insertRecord(const Record &record);
    // bool deleteRecord(const std::string &key);

    BPTree getBPTree() const { return bptree; };
    DiskManager getDiskManager() const { return diskManager; };
    // std::vector<Record> retrieveRecordByBPTree(std::string attributeValue);
    std::vector<Record> retrieveRecordByLinearScan(std::string attributeValue);
    // std::vector<Record> retrieveRangeRecordsByBPTree(std:: string start, std::string end);
    std::vector<Record> retrieveRangeRecordsByLinearScan(std::string start, std::string end);
    // void deleteRecordByBPTree(std::string attributeValue);
    void deleteRecordsByLinearScan(std::string attributeValue);
};

#endif // DATABASE_H
