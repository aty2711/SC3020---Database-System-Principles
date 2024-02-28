#include "disk_manager.h"

std::shared_ptr<Block> DiskManager::readBlock(int blockId)
{
    if (blocks.find(blockId) == blocks.end())
    {
        throw std::runtime_error("Block not found");
    }
    return blocks[blockId];
}

void DiskManager::writeBlock(int blockId, const std::shared_ptr<Block> &block)
{
    blocks[blockId] = block;
}

int DiskManager::createBlock()
{
    if (blocks.size() >= DISK_SIZE / BLOCK_SIZE)
    {
        throw std::runtime_error("Disk is full");
    }
    int blockId = nextBlockId++;
    auto newBlock = std::make_shared<Block>();
    blocks[blockId] = newBlock;
    return blockId;
}

void DiskManager::deleteBlock(int blockId)
{
    if (blocks.find(blockId) == blocks.end())
    {
        throw std::runtime_error("Block not found");
    }
    blocks.erase(blockId);
    return;
}

int DiskManager::getNumRecordsStored() const
{
    int numRecords = 0;
    for (const auto &blockPair : blocks)
    {
        numRecords += blockPair.second->getNumRecordsStored(); // Access the shared_ptr<Block> with blockPair.second
    }
    return numRecords;
}

std::vector<int> DiskManager::getAllBlockIds() const
{
    std::vector<int> blockIds;
    for (const auto &blockPair : blocks)
    {
        blockIds.push_back(blockPair.first);
    }
    return blockIds;
}

double DiskManager::calculateRotationalDelay(int blockId)
{
    int sectorPosition = blockId % (int)sectorsPerTrack; // Find the sector position of the block using modulo for simulation
    double degreesPerSector = 360.0 / sectorsPerTrack;
    double targetAngle = sectorPosition * degreesPerSector; // angle distance from 0 to the target sector

    double currentAngle = 0; // Assume the disk is initially at position 0

    //  Only one direction
    double angularDistance = std::abs(targetAngle - currentAngle); // Calculate the angular distance needed to move

    // Calculate the time needed for the disk to rotate this angular distance
    double rotationsPerMs = rotationalSpeedRPM / 60.0 / 1000.0;                     // Rotations per ms
    double rotationTimeFor360Degrees = 1.0 / rotationsPerMs;                        // Time to rotate 360 degrees in ms
    double rotationalDelay = (angularDistance / 360.0) * rotationTimeFor360Degrees; // Time to rotate the angular distance in ms

    return rotationalDelay;
}

double DiskManager::calculateSeekTime(double distance)
{
    return 3 + (distance / tracksPerSurface) * 2; // 3ms startup time + fraction of full movement * time taken for full movement
    // referring to lecture, average seek time is 4ms. So, we can use 3ms as startup time and 2ms as time taken for full movement
    // 4ms = 3ms + 0.5 * 2ms
}

int DiskManager::blockIdToTrack(int blockId)
{
    return fmod(blockId, tracksPerSurface);
}

double DiskManager::simulateBlockAccessTime(int blockId)
{
    // Simulate cache access time
    bool isCacheHit = (rand() % 100) < (cacheHitRate * 100);
    if (isCacheHit)
    {
        return averageCacheAccessTime; // Return cache access time if hit
    }

    // Simulate track seek time based on distance
    double distance = std::abs(currentHeadPosition - blockIdToTrack(blockId));
    double seekTime = calculateSeekTime(distance);

    // Simulate rotational delay
    double rotationalDelay = calculateRotationalDelay(blockId);

    // Transfer time based on block size and transfer rate
    double transferRateMBperMS = 100.0 / 1000;               // 100 MB/s in MB/ms
    double blockSizeMB = 200.0 / (1024 * 1024);              // 200 bytes in MB
    double transferTime = blockSizeMB / transferRateMBperMS; // Transfer time in ms

    // Update current head position for next access
    currentHeadPosition = blockIdToTrack(blockId);

    // Return total access time in milliseconds
    return seekTime + rotationalDelay + transferTime;
}