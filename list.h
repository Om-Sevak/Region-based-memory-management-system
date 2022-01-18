#ifndef _LIST_H
#define _LIST_H
typedef enum BOOL { false, true } boolean;
typedef struct LIST_BLOCK List_block;
typedef struct LIST_REGIONS List_regions;
#include "blocks.h"
typedef struct NODE_BLOCK Node_block;
typedef struct NODE_REGIONS Node_region;
typedef struct NODE_BLOCK{
    Blocks *block;
    Node_block *next;
}Node_block ;

typedef struct NODE_REGIONS{
    Regions *reg;
    Node_region *next;
}Node_region ;

struct LIST_BLOCK{
    Node_block *top;
    int size;
};
List_regions *construct_regions();
List_block  *construct_blocks();

struct LIST_REGIONS{
    Node_region *top;
    int size;
};
boolean insert_region(List_regions *list,Regions *reg);
Regions * find_region(List_regions*,const char*);
boolean insert_block(List_block *,void *,rsize_t );
boolean delete_block( List_block *, void*);
boolean delete_region(List_regions *list, const char *name);
Blocks * get_block(List_block *list, void *address);
#endif
