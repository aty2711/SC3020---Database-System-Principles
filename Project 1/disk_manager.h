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
    int nextBlockId;                                        // For Block creation and ID assignment                                      // For Block creation and ID assignment

    // Disk Configs
    int numOfSurface;
    int blocksPerSector;  // 2 blocks per sector as an arbitrary number
    int bytesPerSector;   // Calculated based on blocksPerSector * BLOCK_SIZE
    int sectorsPerTrack;  // 256 sectors as an arbitrary number
    int tracksPerSurface; // Calculated based on disk size

    int currentHeadPosition;       // Represents the current position of the disk head
    double rotationalSpeedRPM;     // Rotational speed of the disk in RPM
    double cacheHitRate;           // Percentage of times data is found in cache
    double averageCacheAccessTime; // Average access time from cache in ms

    void updateDiskConfigurations();
    double calculateRotationalDelay(int blockId);
    double calculateSeekTime(double distance);
    int blockIdToTrack(int blockId);

public:
    static const int BLOCK_SIZE = 200; // Size of each block in bytes
    int DISK_SIZE;                     // Size of the disk in bytes

    DiskManager(int DISK_SIZE);

    // std::shared_ptr<Block> readBlock(int blockId);

    Block readBlock(int blockId) const;
    void writeBlock(int blockId, Block block);

    int createBlock();
    void deleteBlock(int blockId);
    int getNumRecordsStored() const;
    int getNumBlocksUsed() const { return blocks.size(); };
    int getTotalBlockCapacity() const { return DISK_SIZE / BLOCK_SIZE; };
    std::vector<int> getAllBlockIds() const;
    double simulateBlockAccessTime(int blockId);
};

#endif // DISK_MANAGER_H
