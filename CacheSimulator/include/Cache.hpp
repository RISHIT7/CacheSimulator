#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <map>

struct MemoryAccess
{
    std::string loadStore;
    unsigned long long int address;
};

struct CacheConstruct
{
    bool dirty, valid;
    unsigned long long int tag, lruPosition, fifoCount;
};

// Cache class
class Cache
{
private:
    unsigned long long int setsNum;
    unsigned long long int blocksPerSet;
    unsigned long long int blockSize;
    std::string writeHitPolicy;
    std::string writeMissPolicy;
    std::string replacementPolicy;

    friend MemoryAccess stringToMemAccess(const std::string trace);

    std::vector<MemoryAccess> accessList;
    std::vector<std::vector<CacheConstruct>> cache;

public:
    // Constructor
    Cache(unsigned long long int sets_number, unsigned long long int blocks_per_set, unsigned long long int block_size, std::string write_hit_policy, std::string write_miss_policy, std::string replacement_policy);

    // Access memory function
    void parseTrace(const std::string trace);

    // Memory access
    void memoryAccess();
    bool read(MemoryAccess access, unsigned long long int indexMask, unsigned long long int instructionCount, long long int &totalCycles);
    bool write(MemoryAccess access, unsigned long long int indexMask, unsigned long long int instructionCount, long long int &totalCycles);

    // replacement policies updateLRU updateFIFO
    void updateLRU(int index, int blockIndex);

    // Destructor
    ~Cache();
};