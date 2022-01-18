# Region-based-memory-management-system

Memory regions are a technique that can supplement the standard C memory allocation system, making it more efficient to allocate and free memory, and to prevent memory leaks. I implemented named memory regions where each region is given a name (a string) and referred to by that name. Each region has a fixed maximum size that is specified when it is created. Within each region, blocks of memory can be allocated and freed, totalling up to its maximum size
