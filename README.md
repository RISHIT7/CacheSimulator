# Cache Simulator 

## Table of Contents

- [Introduction](#introduction)
- [Assumptions](#assumptions)
- [Usage](#usage)
    - [Compilation](#compilation)
    - [Execution](#execution)

## Introduction

The Cache Simulator project provides a comprehensive tool for analyzing the performance of different cache configurations and policies. This simulator allows users to specify a range of cache design parameters, including the number of sets, blocks per set, block size, and cache policies such as write-allocate, write-through/write-back, and replacement strategies (LRU/FIFO).

## Assumptions

The simulator operates under the following assumptions:

- **Processor Cycles**: Loads and stores to/from the cache require one processor cycle, while memory accesses (loads/stores) require 100 processor cycles for each 4-byte quantity transferred.

## Usage

### Compilation

To compile and link the program, execute the following command:

```shell
make
```

This will generate the executable for the cache simulator.

To remove all object files and executables, use the following command:

```shell
make clean
```

### Execution

To execute the simulator, use the following command format:

```shell
./cacheSim <num_sets> <block_size> <blocks_per_set> <write_policy> <allocation_policy> <replacement_policy> < trace_file
```

- `<num_sets>`: Number of cache sets.
- `<block_size>`: Size of each cache block.
- `<blocks_per_set>`: Number of blocks per set.
- `<write_policy>`: Write-through or write-back policy.
- `<allocation_policy>`: Write-allocate or no-write-allocate policy.
- `<replacement_policy>`: LRU (Least Recently Used) or FIFO (First In, First Out) replacement strategy.
- `<trace_file>`: Path to the trace file.

**Example:**

```shell
./cacheSim 1024 32 16 write-allocate write-back lru < path/to/tracefile
```
