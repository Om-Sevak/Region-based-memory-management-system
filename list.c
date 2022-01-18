#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"


boolean is_unique(List_regions *, Regions*);
void validate_regions_list(List_regions *list);
void validate_blocks_list(List_block *list);

List_regions * construct_regions(){
    List_regions *list;
    list = malloc(sizeof(List_regions));
    list->size = 0;
    list->top= NULL;
    assert(list != NULL);
    return list;
}

List_block * construct_blocks(){
    List_block *list;
    list = malloc(sizeof(List_block));
    list->top = NULL;
    list->size = 0;
    assert(list != NULL);
    return list;
}

boolean insert_region(List_regions *list, Regions *reg){
    boolean successful = false;
    assert(list != NULL);
    assert(reg != NULL);
    validate_regions_list(list);

    if(NULL != list) {
        if (is_unique(list, reg)) {
            Node_region *newNode = malloc(sizeof(Node_region));
            newNode->reg = reg;

            //inserting new region in the beginning of the list.
            newNode->next = list->top;
            list->top = newNode;
            successful = true;
        }
    }
    return successful;
}

boolean is_unique(List_regions *list,Regions *temp){
    assert(list != NULL);
    assert(temp != NULL);
    boolean is_unique = false;
    if(NULL != list) {
        Node_region *curr = list->top;
         is_unique = true;
        while (NULL != curr) {
            if (strcmp(curr->reg->name, temp->name) == 0) {
                is_unique = false;
            }
            curr = curr->next;
        }
    }
    return is_unique;
}

Regions * find_region(List_regions *list, const char * region_name){
    assert(list != NULL);
    assert(region_name != NULL);
    validate_regions_list(list);

    Regions *reg = NULL;
    if(NULL != list && NULL != region_name) {
        Node_region *curr = list->top;
        while (NULL != curr) {
            if (strcmp(curr->reg->name, region_name) == 0) {
                reg = curr->reg;
            }
            curr = curr->next;
        }
    }
    return reg;
}

boolean  insert_block(List_block *list,void *address, rsize_t size) {

    assert(list != NULL);
    assert(address != NULL);
    validate_blocks_list(list);

    boolean successful = false;

    if(NULL != list && NULL != address) {

        Blocks *block = malloc(sizeof(Blocks));
        block->adress = address;
        block->size = size;

        if (list->top == NULL) {
            Node_block *newNode = malloc(sizeof(Node_region));
            newNode->block = block;
            newNode->next = NULL;
            list->top = newNode;
            successful = true;
        } else {
            Node_block *current = list->top;
            Node_block *previous = NULL;
            boolean stop = false;
            while (current != NULL && !stop) {
                if (current->block->adress > block->adress) {
                    stop = true;
                } else {
                    previous = current;
                    current = current->next;
                }
            }
            Node_block *temp = malloc(sizeof(Node_block));
            temp->block = block;
            if (previous == NULL) {
                temp->next = list->top;
                list->top = temp;
            } else {
                temp->next = current;
                previous->next = temp;
            }
            successful = true;
        }
    }

    return  successful;
}

boolean delete_block(List_block *list ,void * address){

    assert(list != NULL);
    assert(address != NULL);
    validate_blocks_list(list);

    boolean successful = false;
    if(NULL != list && NULL != address) {
        Node_block *curr = list->top;
        Node_block *prev = NULL;

        while (NULL != curr && curr->block->adress != address) {
            prev = curr;
            curr = curr->next;
        }

        if (curr != NULL) {
            if (prev != NULL) {
                prev->next = curr->next;
            } else {
                list->top = list->top->next;
            }
            successful = true;
        }
    }
    validate_blocks_list(list);
    return successful;
}

boolean delete_region(List_regions *list, const char *name) {

    assert(list != NULL);
    assert(name != NULL);
    validate_regions_list(list);

    boolean successful = false;
    if(NULL != list && NULL != name) {
        Node_region *curr = list->top;
        Node_region *prev = NULL;

        while (NULL != curr && strcmp(curr->reg->name, name) != 0) {
            prev = curr;
            curr = curr->next;
        }
        if (curr != NULL) {
            if (prev != NULL) {
                prev->next = curr->next;
            } else {
                list->top = list->top->next;
            }
            free(curr->reg->region);
            successful = true;
        }
    }
    validate_regions_list(list);
    return successful;
}

 Blocks * get_block(List_block *list, void *address){
     assert(list != NULL);
     assert(address != NULL);
     validate_blocks_list(list);

     Blocks *result = NULL;
     if(NULL != list && NULL != address) {
         Node_block *curr = list->top;

         while (NULL != curr && curr->block->adress != address) {
             curr = curr->next;
         }
         if (NULL != curr) {
             result = curr->block;
         }
     }

    return result;
}

void validate_regions_list(List_regions *list){

    assert(list != NULL);
    assert(list->size >= 0);
#ifndef NDEBUG
    if(NULL != list) {
        Node_region *curr = list->top;
        Node_region *prev = NULL;

        while (NULL != curr) {

            assert(curr->reg != NULL);
            assert(curr->reg->region != NULL);
            assert(curr->reg->size >=0 && curr->reg->total_size > 0 );
            assert(curr->reg->name != NULL);

            if (NULL != prev && NULL != curr) {
                assert(strcmp(curr->reg->name, prev->reg->name) != 0);
            }
            prev = curr;
            curr = curr->next;
        }
    }
#endif
}

void validate_blocks_list(List_block *list){
    assert(list != NULL);
    assert(list->size >= 0);

#ifndef NDEBUG
    if(NULL != list){
        Node_block *curr = list->top;
        Node_block *prev = NULL;

        while(NULL != curr){

            assert(curr->block != NULL);
            assert(curr->block->size > 0);
            assert(curr->block->adress != NULL);
            if(NULL != prev){
                assert(curr->block->adress > prev->block->adress);
            }
            prev = curr;
            curr = curr->next;
        }
    }
#endif
}
