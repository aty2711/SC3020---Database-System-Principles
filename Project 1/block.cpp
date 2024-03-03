#include "block.h"
#include "record.h"
#include <iostream>

bool Block::insertRecord(const Record &record, int index)
{
    // Check if the index is within bounds and the slot is unoccupied
    if (index >= 0 && index < BLOCK_CAPACITY && !slotsOccupancy.test(index))
    {
        records[index] = record;   // Insert the record
        slotsOccupancy.set(index); // Mark the slot as occupied
        return true;               // Indicate successful insertion
    }
    return false; // Slot is already occupied or index out of bounds
}

bool Block::deleteRecord(int index)
{
    // Check if the index is within bounds and the slot is occupied
    if (index >= 0 && index < BLOCK_CAPACITY && slotsOccupancy.test(index))
    {
        records[index] = std ::nullopt; // Delete the record
        slotsOccupancy.reset(index);    // Mark the slot as unoccupied
        return true;                    // Indicate successful deletion
    }
    return false; // Slot is already unoccupied or index out of bounds
}

bool Block::updateRecord(int index, const Record &record)
{
    // Check if the index is within bounds and the slot is occupied
    if (index >= 0 && index < BLOCK_CAPACITY && slotsOccupancy.test(index))
    {
        records[index] = record; // Update the record
        return true;             // Indicate successful update
    }
    return false; // Slot is unoccupied or index out of bounds
}

void Block::printBlock() const
{
    std::cout << "Block contents:" << std::endl;
    for (int i = 0; i < BLOCK_CAPACITY; i++)
    {
        if (records[i] == std::nullopt)
        {

            std::cout << "Slot " << i << " is empty" << std::endl;
        }
        else
        {
            records[i]->print();
        }
    }
    std::cout << std::endl;
}

int Block::getNumRecordsStored() const
{
    return slotsOccupancy.count();
}

int Block::getFreeIndex() const
{
    for (int i = 0; i < BLOCK_CAPACITY; i++)
    {
        if (!slotsOccupancy.test(i))
        {
            return i;
        }
    }
    return -1; // No free slots available
}

std::vector<Record> Block::retrieveAllRecords() const
{
    std::vector<Record> allRecords;
    for (int i = 0; i < BLOCK_CAPACITY; i++)
    {
        if (slotsOccupancy.test(i))
        {
            allRecords.push_back(*records[i]);
        }
    }
    return allRecords;
}

Record Block::retrieveRecord(int index) const
{
    if (index >= 0 && index < BLOCK_CAPACITY && slotsOccupancy.test(index))
    {
        return *records[index];
    }
    throw std::runtime_error("Record not found");
}