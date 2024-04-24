#include <iostream>
#include "../include/Cache.hpp"

int main(int argc, char **argv)
{
    // Checking if all the inputs are preesent
    if (argc != 7)
    {
        std::cerr << "Usage: \n./cacheSim <no_sets> <no_blocks_per_set> <block_size> <write_policy> <write_strategy> <replacement_policy> < <trace_file>" << std::endl;
    }

    int setNum = std::stoi(argv[1]);
    int blocksPerSet = std::stoi(argv[2]);
    int blockSize = std::stoi(argv[3]);
    std::string writeMissPolicy = argv[4];
    std::string writeHitPolicy = argv[5];
    std::string replacementPolicy = argv[6];

    Cache* cache = new Cache(setNum, blocksPerSet, blockSize, writeHitPolicy, writeMissPolicy, replacementPolicy);
    
    std::string traceLine;
    
    while (std::getline(std::cin, traceLine)) {
        cache->parseTrace(traceLine);
    }
    cache->memoryAccess();

    delete cache;
    cache = nullptr;

    return 0;
}