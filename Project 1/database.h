#ifndef DATABASE_H
#define DATABASE_H

#include "block.h"

typedef unsigned int uint;
typedef unsigned char uchar;

class Database
{
private:
    uchar *databasePtr;
    uchar *blkPtr;

    uint databaseSize;
    uint databaseUsedBlks;
    uint databaseUsedRecords;
    uint curBlkUsed;

    int numAllocBlks;
    int numAvailBlks;

public:

    Database(uint databaseSize);

    ~Database();

    uint getDatabaseSize()
    {
        return databaseSize;
    }

    int getDatabaseUsedBlks()
    {
        return databaseUsedBlks;
    }

    int getDatabaseUsedRecords()
    {
        return databaseUsedRecords;
    }


    int getCurBlkUsed()
    {
        return curBlkUsed;
    }


    int getNumAllocBlks()
    {
        return numAllocBlks;
    }


    int getNumAvailBlks()
    {
        return numAvailBlks;
    }

    bool addNewBlock();
    bool addRecord(const Record &record);
    bool deleteRecord(uchar *blkAddress, uint relativeOffset, uint recordSize);
};

#endif // DATABASE_H
