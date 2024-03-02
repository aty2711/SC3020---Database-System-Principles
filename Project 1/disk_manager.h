/**
 * @file disk_manager.h
 * @brief Defines the DiskManager class for managing disk storage and block allocation.
 *
 * This class simulates disk storage to simulate block read and write operations,
 * block allocation and deallocation, and disk space management. It provides a simplified
 * model of disk operations, including simulating tracks, sectors, and blocks, as well as simulating
 * access times and disk seek times. Blocks are accessed at the block level, using an abstraction
 * that allows for the simulation of more complex disk storage behaviors.
 *
 * We will use non-sequential storage of data blocks. This is because the B+ tree implementation
 * will be used to reduce our read time. Meanwhile, using non-sequential storage will allow us to
 * simplify implementation of writing and deleting blocks.
 */

#ifndef DISK_MANAGER_H
#define DISK_MANAGER_H

#include "block.h"
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <memory>

class DiskManager
{
private:
    std::unordered_map<int, std::shared_ptr<Block>> blocks; // Maps block IDs to Block objects
    int nextBlockId;                                        // For Block creation and ID assignment

    // Disk Configs
    int numOfSurface = 1;
    int blocksPerSector = 2;                                                            // 2 blocks per sector as an arbitrary number
    int bytesPerSector = blocksPerSector * BLOCK_SIZE;                                  // 400 bytes per sector
    int sectorsPerTrack = 256;                                                          // 256 sectors as an arbitrary number
    int tracksPerSurface = DISK_SIZE / sectorsPerTrack / bytesPerSector / numOfSurface; // 5120 tracks * 256 sectors * 400 bytes = 524288000 bytes = 500MB

    int currentHeadPosition = 0;           // Represents the current position of the disk head
    double rotationalSpeedRPM = 5400;      // Rotational speed of the disk in RPM
    double cacheHitRate = 0.1;             // Percentage of times data is found in cache
    double averageCacheAccessTime = 0.001; // Average access time from cache in ms

    double calculateSeekTime(double distance);
    double calculateRotationalDelay(int blockId);
    int blockIdToTrack(int blockId);

public:
    static const int BLOCK_SIZE = 200; // Size of each block in bytes
    int DISK_SIZE;                     // Size of the disk in bytes

    DiskManager(int DISK_SIZE) : nextBlockId(0)
    {
        this->DISK_SIZE = DISK_SIZE;
    }

    std::shared_ptr<Block> readBlock(int blockId);

    void writeBlock(int blockId, const std::shared_ptr<Block> &block);
    int createBlock();
    void deleteBlock(int blockId);
    int getNumRecordsStored() const;
    int getTotalBlockCapacity() const { return DISK_SIZE / BLOCK_SIZE; };
    std::vector<int> getAllBlockIds() const;

    double simulateBlockAccessTime(int blockId);
};

#endif // DISK_MANAGER_H
