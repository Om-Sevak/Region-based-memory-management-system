#ifndef _BLOCKS_H
#define _BLOCKS_H
#include "regions.h"
#include "list.h"
typedef struct BLOCKS{
    void *adress;
    rsize_t size;
}Blocks;

typedef struct REGIONS{
    const char *name;
    void *region;
    List_block *block;
    rsize_t total_size;
    rsize_t size;
}Regions;

#endif //_BLOCKS_H
