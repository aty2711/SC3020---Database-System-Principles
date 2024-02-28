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
    // Model assumes blocks are evenly distributed across the disk and the disk rotates at a constant speed.
    int totalBlocks = getTotalBlockCapacity();
    double blocksPerTrack = static_cast<double>(totalBlocks) / numTracks; // Should be 10732/1024 = 10.5

    int sectorPosition = blockId % (int)blocksPerTrack; // Calculate the position of the block on the track
    double degreesPerSector = 360.0 / blocksPerTrack;   // 360 degrees divided by blocks per track, assuming 1 block per sector
    double targetAngle = sectorPosition * degreesPerSector;

    double currentAngle = 0; // Assume the disk is initially at position 0

    double angularDistance = std::abs(targetAngle - currentAngle); // Calculate the angular distance needed to move

    // Adjust for the shortest path (clockwise or counterclockwise)
    if (angularDistance > 180)
    {
        angularDistance = 360 - angularDistance;
    }

    // Calculate the time needed for the disk to rotate this angular distance
    double rotationsPerMs = rotationalSpeedRPM / 60.0 / 1000.0;                     // Rotations per ms
    double rotationTimeFor360Degrees = 1.0 / rotationsPerMs;                        // Time to rotate 360 degrees in ms
    double rotationalDelay = (angularDistance / 360.0) * rotationTimeFor360Degrees; // Time to rotate the angular distance in ms

    return rotationalDelay;
}

double DiskManager::calculateSeekTime(double distance)
{
    // Simple linear model
    return 0.004 + (distance / numTracks) * 0.010; // Base time plus additional time per block
}

int DiskManager::blockIdToTrack(int blockId)
{
    return fmod(blockId, numTracks);
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