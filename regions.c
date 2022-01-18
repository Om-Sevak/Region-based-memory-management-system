#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "regions.h"
#include "list.h"

static List_regions *list;

static Regions *choosen_region;

void initiate_mem(void* , rsize_t size);
rsize_t my_round(rsize_t);
void validate_region(Regions *reg);

Boolean rinit(const char *region_name, rsize_t region_size){

    assert(region_name != NULL);
    assert(region_size > 0);

    boolean successful = false;

    if(NULL != region_name && region_size > 0) {

        if (list == NULL) {
            list = construct_regions();
        }
        Regions *reg = malloc(sizeof(Regions));

        reg->name = malloc(sizeof(region_name) + 1);
        strcpy((char *) reg->name, region_name);
        reg->region = malloc(region_size);
        reg->size = my_round(region_size);
        reg->total_size = my_round(region_size);
        reg->block = construct_blocks();

        validate_region(reg);

        successful = insert_region(list, reg);

        if(successful) {
            choosen_region = reg;
        }
    }
    return (Boolean) successful;
}

Boolean rchoose(const char *region_name)
{
    assert(region_name != NULL);

    boolean successful = false;
    if(NULL != region_name) {
        Regions *temp = find_region(list, region_name);

        if (NULL != temp) {
            choosen_region = temp;
            successful = true;
        }
    }

    return (Boolean) successful;
}

const char * rchosen(){

    validate_region(choosen_region);

    if(NULL != choosen_region && NULL != choosen_region->name ) {
        return choosen_region->name;
    } else {
        return NULL;
    }
}


void *ralloc(rsize_t block_size){

    assert(block_size > 0);
    void *result = NULL;

    if(block_size > 0) {
        if(block_size > 0 && block_size < 4){
            block_size = 8;
        }else{
            block_size = my_round(block_size);
        }
        block_size = my_round(block_size);
        rsize_t temp_size = block_size;
        rsize_t availabe_mem = block_size - 1;
        rsize_t last_chunck;

        validate_region(choosen_region);
        void *last = (choosen_region->region) + (choosen_region->total_size);

        if (block_size != 0 && block_size <= choosen_region->size) {
            Node_block *curr = choosen_region->block->top;
            Node_block *prev = NULL;
            //if it's the first ralloc
            //i.e. if the top of block list is null
            if (NULL == curr) {
                result = choosen_region->region;
            //if the first block is freed and available of ralloc
            //that is, top is not pointing to first block, so it would be freed
            } else if (curr->block->adress != choosen_region->region) {
                //calculating available memory in first block
                availabe_mem = (choosen_region->block->top->block->adress) - (choosen_region->region);
                //checking if we got enough memory
                if (block_size <= availabe_mem)
                    result = choosen_region->region;

            } else {
                // will run until we have enough available memory
                while (NULL != curr && block_size > availabe_mem) {

                    if (NULL != curr) {
                        prev = curr;
                        curr = curr->next;
                        //in the case where we are at the end of list of blocks
                        if (NULL == curr && prev != NULL) {
                            //so calculating if the last chunck of memory has enough memory
                            //by subtracting last used pointer to the last point of block of memory
                            last_chunck = (last) - ((prev->block->adress) + (prev->block->size));
                            //if last chunck of memory has enough space for new block, get the pointer
                            if (last_chunck >= block_size)
                                result = ((prev->block->adress) + ((prev->block->size)));

                        } else {
                            //calculating availabe memory between two blocks
                            temp_size = (curr->block->adress) - (prev->block->adress);
                            availabe_mem = (temp_size) - (prev->block->size);
                            // if enough memory is available, get the pointer
                            if (availabe_mem >= block_size)
                                result = (prev->block->adress) + (prev->block->size);
                        }
                    }
                }
            }
            //from above search, if we had any available memory,
            //result would not be null and so we will create a block with
            //result and choose the region and initiate memory to zero
            if (NULL != result) {
                validate_region(choosen_region);
                insert_block(choosen_region->block, result, block_size);
                choosen_region->size -= block_size;
                initiate_mem(result, block_size);
            }
        }
    }
    return result;
}


void initiate_mem(void * mem_block,rsize_t size){
    char *limit = mem_block + size;
    char *i;
    for(i = mem_block; i < limit; i++){
        *i = 0;
   }
}

rsize_t  rsize(void *block_ptr){

     validate_region(choosen_region);
     Blocks *temp;
     rsize_t result = 0;

     if(NULL != block_ptr) {
         if (NULL != choosen_region) {
             temp = get_block(choosen_region->block, block_ptr);
             if (NULL != temp) {
                 result = temp->size;
             }
         }
     }

     return result;
}

Boolean rfree(void *block_ptr){

    Boolean  successful = FALSE;

    if(NULL != block_ptr) {
        rsize_t block_size = rsize(block_ptr);
        successful = (Boolean) delete_block(choosen_region->block, block_ptr);
        if (successful) {
            choosen_region->size += block_size;
        }
    }

    return successful;
}

void rdestroy(const char *region_name){

    assert(region_name != NULL);

    if(NULL != region_name) {
        if (NULL != choosen_region && strcmp(choosen_region->name, region_name) == 0) {
            choosen_region = NULL;
        }
        delete_region(list, region_name);
    }
}
void rdump(){

    if(NULL != list) {
        Node_region *curr_region = list->top;
        Node_block *curr_block;
        rsize_t used_space = 0;
        rsize_t free_space;
        rsize_t free_percent;
        if (NULL != curr_region) {
            while (NULL != curr_region) {
                used_space = 0;
                free_space = 0;
                free_percent = 0;


                printf("*** Region: %s \n", curr_region->reg->name);
                curr_block = curr_region->reg->block->top;
                while (NULL != curr_block) {
                    printf("\t%p : %d \n", (unsigned int *) (curr_block->block->adress), curr_block->block->size);
                    used_space += curr_block->block->size;
                    curr_block = curr_block->next;
                }
                free_space = (curr_region->reg->total_size) - used_space;
                free_percent = (free_space * 100) / curr_region->reg->total_size;
                printf("Free space: %d%% \n", free_percent);
                curr_region = curr_region->next;
            }
        } else {
            printf("No regions \n");
        }
    }
}


rsize_t my_round(rsize_t n)
{
    assert(n >= 0);

    return ((n + 7) & (-8));
}

void validate_region(Regions *reg){
    assert(reg != NULL);
    assert(reg->block != NULL);
    assert(reg->name != NULL);
    assert(reg->size >= 0);
    assert(reg->total_size > 0);
}

