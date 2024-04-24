#include "../include/Cache.hpp"

MemoryAccess stringToMemAccess(const std::string trace)
{
    MemoryAccess access;
    access.loadStore = trace[0];
    access.address = std::stoull(trace.substr(2, 10), nullptr, 16);

    return access;
}

void Cache::updateLRU(int index, int blockIndex)
{
    unsigned long long int maxLRUCounter = 0;

    for (unsigned long long i = 0; i < blocksPerSet; i++)
    {
        if (i != maxLRUCounter && cache[index][i].lruPosition > maxLRUCounter)
        {
            maxLRUCounter = cache[index][i].lruPosition;
        }
    }

    cache[index][blockIndex].lruPosition = maxLRUCounter + 1;
}

bool Cache::read(MemoryAccess access, unsigned long long int indexMask, unsigned long long int instructionCount, long long int &totalCycles)
{
    unsigned long long int index = (access.address / blockSize) & indexMask;
    unsigned long long int tag = (access.address / (blockSize * (indexMask + 1)));

    for (unsigned long long int i = 0; i < blocksPerSet; i++)
    {
        if (cache[index][i].valid && cache[index][i].tag == tag)
        {
            // Hit
            updateLRU(index, i);
            return true;
        }
    }

    // Miss
    for (unsigned long long int i = 0; i < blocksPerSet; i++)
    {
        if (!cache[index][i].valid)
        {
            // Found Empty
            cache[index][i].dirty = false;
            cache[index][i].valid = true;
            cache[index][i].tag = tag;
            cache[index][i].lruPosition = 0;
            cache[index][i].fifoCount = instructionCount;
            updateLRU(index, i);
            break;
        }
    }
    // Did not find any empty positions

    // implement lru and fifo
    unsigned long long int counter = instructionCount;
    unsigned long long int victimBlock = 0;
    if (replacementPolicy == "lru")
    {
        // lru replacement
        for (unsigned long long int i = 0; i < blocksPerSet; i++)
        {
            if (counter > cache[index][i].lruPosition)
            {
                counter = cache[index][i].lruPosition;
                victimBlock = i;
            }
        }

        // at that set index, we look at a block index, with min lru_position, if dirty bit is 1 then total cycles += 100 from default
        if (cache[index][victimBlock].dirty)
        {
            totalCycles += 1 * (blockSize / 4);
        }

        cache[index][victimBlock].dirty = false;
        cache[index][victimBlock].valid = true;
        cache[index][victimBlock].tag = tag;
        cache[index][victimBlock].lruPosition = 0;
        cache[index][victimBlock].fifoCount = instructionCount;
    }
    else
    {
        // fifo replacement
        for (unsigned long long int i = 0; i < blocksPerSet; i++)
        {
            if (counter > cache[index][i].fifoCount)
            {
                counter = cache[index][i].fifoCount;
                victimBlock = i;
            }
        }

        // at that set index, we look at a block with highest fifo_count, if dirty bit is 1 then total cycles += 100
        if (cache[index][victimBlock].dirty)
        {
            totalCycles += 100 * (blockSize / 4);
        }

        cache[index][victimBlock].dirty = false;
        cache[index][victimBlock].valid = true;
        cache[index][victimBlock].tag = tag;
        cache[index][victimBlock].lruPosition = 0;
        cache[index][victimBlock].fifoCount = instructionCount;
    }

    return false;
}

bool Cache::write(MemoryAccess access, unsigned long long int indexMask, unsigned long long int instructionCount, long long int &totalCycles)
{
    unsigned long long int index = (access.address / blockSize) & indexMask;
    unsigned long long int tag = (access.address / (blockSize * (indexMask + 1)));

    for (unsigned long long int i = 0; i < blocksPerSet; i++)
    {
        if (cache[index][i].valid && cache[index][i].tag == tag)
        {
            // Hit
            if (writeHitPolicy == "write-back")
            {
                // setting the dirty bit to true for write-back
                if (writeMissPolicy == "write-allocate")
                    cache[index][i].dirty = true;
                else
                    cache[index][i].dirty = false;                    
            }
            updateLRU(index, i);
            return true;
        }
    }

    // Miss
    if (writeMissPolicy == "write-allocate")
    {
        // write allocate
        for (unsigned long long int i = 0; i < blocksPerSet; i++)
        {
            if (!cache[index][i].valid)
            {
                // Found Empty
                if (writeHitPolicy == "write-back")
                {
                    cache[index][i].dirty = true;
                }
                else
                {
                    cache[index][i].dirty = false;
                }
                cache[index][i].valid = true;
                cache[index][i].tag = tag;
                cache[index][i].lruPosition = 0;
                cache[index][i].fifoCount = instructionCount;
                updateLRU(index, i);

                break;
            }
        }
        // Did not find any empty positions

        // implement lru and fifo
        unsigned long long int counter = instructionCount;
        unsigned long long int victimBlock = 0;
        if (replacementPolicy == "lru")
        {
            // lru replacement
            for (unsigned long long int i = 0; i < blocksPerSet; i++)
            {
                if (counter > cache[index][i].lruPosition)
                {
                    counter = cache[index][i].lruPosition;
                    victimBlock = i;
                }
            }

            // at that set index, we look at a block index, with min lru_position, if dirty bit is 1 then total cycles += 100 from default
            if (cache[index][victimBlock].dirty)
            {
                totalCycles += 100 * (blockSize / 4);
            }

            if (writeHitPolicy == "write-back")
            {
                cache[index][victimBlock].dirty = true;
            }
            else
            {
                cache[index][victimBlock].dirty = false;
            }
            cache[index][victimBlock].valid = true;
            cache[index][victimBlock].tag = tag;
            cache[index][victimBlock].lruPosition = 0;
            cache[index][victimBlock].fifoCount = instructionCount;
        }
        else
        {
            // fifo replacement
            for (unsigned long long int i = 0; i < blocksPerSet; i++)
            {
                if (counter > cache[index][i].fifoCount)
                {
                    counter = cache[index][i].fifoCount;
                    victimBlock = i;
                }
            }

            // at that set index, we look at a block with highest fifo_count, if dirty bit is 1 then total cycles += 100
            if (cache[index][victimBlock].dirty)
            {
                totalCycles += 100 * (blockSize / 4);
            }

            if (writeHitPolicy == "write-back")
            {
                cache[index][victimBlock].dirty = true;
            }
            else
            {
                cache[index][victimBlock].dirty = false;
            }
            cache[index][victimBlock].valid = true;
            cache[index][victimBlock].tag = tag;
            cache[index][victimBlock].lruPosition = 0;
            cache[index][victimBlock].fifoCount = instructionCount;
        }

        return false;
    }
    return false;
}

void Cache::memoryAccess()
{
    unsigned long long int cacheSize = setsNum * blocksPerSet * blockSize;
    // bitwise AND with address will give the indexBits
    unsigned long long int indexMask = (cacheSize / (blocksPerSet * blockSize)) - 1;

    // result variables
    long long int loads = 0, stores = 0, totalCycles = 0;
    long long int storeHit = 0, loadHit = 0, storeMiss = 0, loadMiss = 0;

    unsigned long long int instructionCount = 0;

    for (MemoryAccess access : accessList)
    {
        instructionCount++;
        if (access.loadStore == "l")
        {
            loads++;
            if (read(access, indexMask, instructionCount, totalCycles))
            {
                // load hit
                loadHit++;
                totalCycles += 1; // cache read would take 1 cycle
            }
            else
            {
                // load Miss
                loadMiss++;
                if (writeMissPolicy == "write-allocate")
                {
                    totalCycles += 100 * (blockSize / 4) + 1; // cache copy from memory to cache, then read from cache
                }
                else
                {
                    totalCycles += 100;
                }
            }
        }
        else
        {
            stores++;
            if (write(access, indexMask, instructionCount, totalCycles))
            {
                // store hit
                storeHit++;
                if (writeHitPolicy == "write-back")
                {
                    // since data updated in the cache
                    totalCycles += (blockSize / 4);
                }
                else
                {
                    // considering both updates in the cache and main memory
                    totalCycles += 101 * (blockSize / 4);
                }
            }
            else
            {
                storeMiss++;
                if (writeMissPolicy == "write-allocate")
                {
                    totalCycles += 101 * (blockSize / 4);
                }
                else
                {
                    totalCycles += 100 * (blockSize / 4);
                }
            }
        }
    }
    std::cout << "Total Loads: " << loads << "\nTotal Stores: " << stores << "\nLoad Hits: " << loadHit << "\nLoad Misses: " << loadMiss << "\nStore Hits: " << storeHit << "\nStore Misses: " << storeMiss << "\nTotal Cycles: " << totalCycles << "\n";
}

Cache::Cache(unsigned long long int sets_number, unsigned long long int blocks_per_set, unsigned long long int block_size, std::string write_hit_policy, std::string write_miss_policy, std::string replacement_policy)
{
    setsNum = sets_number;
    blocksPerSet = blocks_per_set;
    blockSize = block_size;

    writeHitPolicy = write_hit_policy;
    writeMissPolicy = write_miss_policy;
    replacementPolicy = replacement_policy;

    // initialise cache
    for (unsigned long long int i = 0; i < setsNum; i++)
    {
        std::vector<CacheConstruct> internal_cache(blocksPerSet);
        cache.push_back(internal_cache);
    }
}

void Cache::parseTrace(const std::string trace)
{
    MemoryAccess access = stringToMemAccess(trace);
    accessList.push_back(access);
}

Cache::~Cache() {}