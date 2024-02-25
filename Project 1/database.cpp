#include "database.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <string.h>
using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;

Database::Database(uint databaseSize)
{
    this->databaseSize = databaseSize;
    uchar *databasePtr = nullptr;
    this->databasePtr = new uchar[databaseSize]; // allocate databaseSize uchars and store it in databasePtr and initialize it to NULL.
    this->blkPtr = nullptr;
    this->databaseUsedBlks = 0;
    this->databaseUsedRecords = 0;
    this->numAllocBlks = 0;
    this->numAvailBlks = databaseSize / 200;
    this->curBlkUsed = 0;
}

Database::~Database()
{
    delete[] this->databasePtr;
    this->databasePtr = nullptr;
}

bool Database::addNewBlock()
{

    if (numAvailBlks > 0)
    {
        blkPtr = databasePtr + (numAllocBlks * 200); // increment blkPtr by number of allocated blocks
        databaseUsedBlks += 200;
        numAvailBlks -= 1; // decrement the available number of blocks by 1
        numAllocBlks += 1; // increment the number of allocated blocks by 1
        curBlkUsed = 0;

        return true;
    }

    else
    {
        cout << "MEMORY FULL";
        return false;
    }
}

bool Database::addRecord(const Record &record)
{
    std::string serializedRecord = record.serialize();

    if (200 < serializedRecord.size())
    {
        throw "Unable to reserve space as record size is greater than the block size";
    }

    if (curBlkUsed + serializedRecord.size() + 1 > 200 || numAllocBlks == 0)
    {
        if (!addNewBlock())
        {
            std::cerr << "Failed to allocate a new block as no free space in blocks or no blocks can be allocated." << std::endl;
            return false; // Unable to allocate a new block
        }
    }

    blkPtr = databasePtr + (numAllocBlks - 1) * 200; // Calculate current block's starting point

    // Copy the serialized record into the block
    memcpy(blkPtr + curBlkUsed, serializedRecord.c_str(), serializedRecord.size());

    // Update the current block usage and global record count
    curBlkUsed += serializedRecord.size() + 1; // +1 for delimiter or end-of-record marker
    databaseUsedRecords += serializedRecord.size() + 1;

    blkPtr[curBlkUsed - 1] = '\0'; // Null-terminate, optional

    // Uncomment for debugging
    // cout << "Record details:" << endl;
    // record.print();
    // cout << "Successfully added to database." << endl;

    return true;
}

bool Database::deleteRecord(uchar *blkAddress, uint relativeOffset, uint recordSize)
{

    try
    {
        // Deallocate used memory from counter (should be done last after the successful deletion)
        // Replace the record with null bytes
        //
        databaseUsedRecords -= recordSize;
        fill(blkAddress + relativeOffset, blkAddress + relativeOffset + recordSize, '\0');

        uchar cmpBlk[recordSize];
        fill(cmpBlk, cmpBlk + recordSize, '\0');

        if (equal(cmpBlk, cmpBlk + recordSize, blkAddress))
        {
            databaseUsedBlks -= 200;
        }

        return true;
    }

    catch (exception &e)
    {
        cout << "Exception" << e.what() << "\n";
        cout << "Delete record or block failed"
             << "\n";
        return false;
    }
}
