/**
 * @file block.h
 * @brief Defines the Block class for managing fixed-size memory blocks to store serialized records.
 *
 * The Block class simulates a fixed-size memory block, specifically designed to store
 * a limited number of records. Each record, typically serialized into a 19-byte format,
 * is represented here by a Record object for simplicity and architecture neutrality.
 * The block is capable of holding up to 10 records, aligning with a fixed block size of 200 bytes.
 * This class leverages a std::array of std::optional<Record> to simulate the block storage,
 * allowing for efficient management of occupied and unoccupied slots within the block. The occupancy
 * of each slot is tracked using a std::bitset, enabling quick checks and updates for record
 * insertion, deletion, and updates. This design facilitates block management and record inspection
 * while maintaining alignment and efficient access within the simulated environment.
 */

#ifndef BLOCK_H
#define BLOCK_H

#include "record.h"
#include <vector>
#include <string>
#include <bitset>
#include <optional>
#include <array>

class Block
{
public:
    // Block metadata
    static const int BLOCK_CAPACITY = 200 / sizeof(Record); // Maximum number of records in a block
    std::bitset<BLOCK_CAPACITY> slotsOccupancy;             // All bits are 0 (empty) by default

    Block()
    {
        slotsOccupancy.reset(); // Ensure all slots are marked as empty initially
    }

    bool insertRecord(const Record &record, int index);
    bool deleteRecord(int index);
    bool updateRecord(int index, const Record &record);
    void printBlock() const;
    std::array<std::optional<Record>, BLOCK_CAPACITY> getRecords() const { return records; }
    int getNumRecordsStored() const;
    int getFreeIndex() const; // Returns the index of the first free slot, or -1 if the block is full
    std::vector<Record> retrieveAllRecords() const;

private:
    std::array<std::optional<Record>, BLOCK_CAPACITY> records;
};

#endif // BLOCK_H
