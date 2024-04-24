**Cache Simulator**

**Introduction:**
This cache simulator, developed by Rishit Jakharia (2022CS11621), is designed to analyze various cache configurations and their performance. The simulator allows users to specify cache design parameters such as the number of sets, blocks per set, block size, and different cache policies including write-allocate, write-through/write-back, and replacement policies (LRU/FIFO).

**Assumptions:**
The simulator operates under the assumption that loads/stores from/to the cache take one processor cycle, while loads/stores from/to memory take 100 processor cycles for each 4-byte quantity transferred.

**Usage:**
To compile and link the program, execute `make`. To remove all object files and executables, execute `make clean`. \
One Example is show below
```
 ./cacheSim 1024 32 16 write-allocate write-back lru < path/to/tracefile
```

**Best Configuration:**
  - The best configuration observed is 32-way Set Associative with 1024 sets, employing write-back, write-allocate, and LRU replacement policy.

**Conclusion:**
Based on the analysis, the optimal configuration for the cache simulator is determined to be 32-way Set Associative with 1024 sets, utilizing write-back, write-allocate, and LRU replacement policy.

**Note:**
Ensure that the provided parameters adhere to the specified constraints and assumptions for accurate simulation results.