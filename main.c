/**
 * main
 * AUTHOR       Om sevak, 007942904
 * DATE          December,13 2021
 *
 * PURPOSE: This program test's Region based memory management system.
 */


#include <stdio.h>
#include <string.h>
#include "regions.h"

void test_rsize(void *blockPtr, rsize_t expected);
void test_rchoosen(const char *choosen, Boolean expected);
void* test_ralloc(rsize_t size, Boolean expected);
void test_rinit(const char* reg_name, rsize_t size, Boolean expected);
void test_initiated_memory(void *block);
void test_rfree(void *ptr , Boolean expected);
void test_first_fit(void*, void*);
void test_typical_cases();
void test_first_fit_cases();
void test_special_cases();
void test_edge_cases();
void test_invalid_cases();


int total_test_executed = 0;
int failed_test = 0;

int main()
{
    test_typical_cases();
    test_first_fit_cases();
    test_special_cases();
    test_edge_cases();
#ifdef NDEBUG
    test_invalid_cases();
#endif

    printf("Concluding: \n");
    printf("Total number of test: %d \n",total_test_executed);
    printf("Total passed test: %d \n", total_test_executed- failed_test);
    printf("Total failed test: %d \n",failed_test);
    return 0;
}

void test_typical_cases(){
    printf("Testing typical cases: \n");
    printf("---------------------------------\n");
    test_rinit("Region1",1000,TRUE);
    test_rinit("Region2",1000,TRUE);
    void *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL,*ptr4 = NULL;
    ptr1=test_ralloc(100,TRUE);
    ptr2=test_ralloc(200,TRUE);
    ptr3=test_ralloc(900,FALSE);
    rchoose("Region1");
    ptr4 = test_ralloc(500,TRUE);
    rdump();
    rchoose("Region2");
    test_rsize(ptr1,104);
    test_rsize(ptr3,0);
    test_rsize(ptr2,200);
    test_rchoosen("Region2",TRUE);
    test_rfree(ptr1,TRUE);
    test_rfree(ptr3,FALSE);

    rdestroy("Region1");
    rdestroy("Region2");
}

void test_first_fit_cases(){

    printf("Testing first fit cases: \n");
    printf("---------------------------------\n");
    test_rinit("First-fit",1000,TRUE);
    void *ptr1, *ptr2, *ptr3,*ptr4;
    Boolean temp;
    ptr1 = ralloc(100);
    ptr2 = ralloc(200);
    ptr3 = ralloc(300);
    ptr4 = ralloc(400);
    temp = rfree(ptr1);
    test_first_fit(ptr1, ralloc(100));
    temp = rfree(ptr2);
    test_first_fit(ptr2, ralloc(200));
    temp = rfree(ptr4);
    test_first_fit(ptr4, ralloc(400));
    temp = rfree(ptr3);
    test_first_fit(ptr3, ralloc(300));
    rdestroy("First-fit");

}

void test_special_cases(){

    printf("Testing special cases: \n");
    printf("---------------------------------\n");
    test_rinit("Region1",10000,TRUE);
    test_rinit("Region2",100,TRUE);
    test_rinit("Region2",1000,FALSE);
    test_rchoosen("Region1",FALSE);
    test_rchoosen("Region2",TRUE);
    test_rinit("   ",1000,TRUE);
    test_rchoosen("   ",TRUE);
    test_rchoosen("doesn't exists",FALSE);
    rdestroy("Region1");
    rdestroy("Region2");
    rdestroy("   ");


}

void test_edge_cases(){
    printf("Testing edge cases: \n");
    printf("---------------------------------\n");

    test_rinit("Region1",1,TRUE);
    void *ptr = test_ralloc(1,TRUE);
    test_rsize(ptr,8);
    void *ptr1 = test_ralloc(1,FALSE);
    test_rsize(ptr1,0);
    rdestroy("Region1");
}

void test_invalid_cases() {

    printf("Testing invalid cases: \n");
    printf("---------------------------------\n");
    test_rinit(NULL, 1000, FALSE);
    test_rinit("Region1", 0, FALSE);
    test_rinit("Region2", 1000, TRUE);
    void *ptr1 = test_ralloc(0, FALSE);
    test_rsize(ptr1, 0);
    test_rsize(NULL,FALSE);
    test_rchoosen(NULL,FALSE);
    test_rfree(NULL,FALSE);
    rdestroy("Region2");
}

void test_rinit(const char* reg_name, rsize_t size, Boolean expected){
    Boolean actual;
    actual = rinit(reg_name,size);
    if(actual == expected){
        printf("Passed: rinit test, region with size: %s ,%d\n",reg_name,((size + 7) & (-8)));
    } else {
        printf("Failed: rinit test, unable to create region with name %s and size %d\n",reg_name,size);
        failed_test++;
    }
    total_test_executed++;
}

void* test_ralloc(rsize_t size, Boolean expected){
    Boolean actual;
    void* ptr = ralloc(size);
    if(NULL == ptr){
        actual = FALSE;
    } else{
        actual = TRUE;
    }
    if(actual == expected){
        printf("Passed: ralloc call was expected\n");
        test_initiated_memory(ptr);
    } else{
        printf("Failed: ralloc call was unexpected \n");
        failed_test++;
    }
    total_test_executed++;
    return ptr;
}

void test_initiated_memory(void *block){
    Boolean is_initiated = TRUE;
    char *ptr = (char*)block;
    rsize_t size = rsize(block);
    int i = 0;
    for( i = 0; i < size && is_initiated; i ++ ){
        if(0 != ptr[i]){
            is_initiated = FALSE;
        }
    }
    if(is_initiated){
        printf("Passed: allocated memory initialized to 0\n");
    } else {
        printf("Failed: allocated memory not initialized to 0");
        failed_test++;
    }

    total_test_executed++;
}

void test_rchoosen(const char *choosen, Boolean expected){
    Boolean actual;
    if(NULL != choosen && strcmp(choosen,rchosen()) == 0){
        actual = TRUE;
    } else {
        actual = FALSE;
    }
    if(actual == expected){
        printf("Passed: rchoose test, chosen region: %s\n",rchosen());
    } else {
        printf("Failed: rchoose test, chosen region: %s\n",rchosen());
        failed_test++;
    }
    total_test_executed++;
}

void test_rsize(void *blockPtr, rsize_t expected){

    rsize_t actual = rsize(blockPtr);
    if(actual == expected){
        printf("Passed: rsize test, size is: %d \n",actual);
    } else{
        printf("Failed: rsize tes, size is %d \n",actual);
        failed_test++;
    }
    total_test_executed++;
}

void test_rfree(void *ptr , Boolean expected){
    Boolean actual = rfree(ptr);
    if(actual == expected){
        printf("Passed: rfree call was expected.\n");
    } else{
        printf("Failed: rfree call was unexpected.\n");
        failed_test++;
    }
    total_test_executed++;
}

void test_first_fit(void *ptr1, void *ptr2){
    if(ptr1 == ptr2){
        printf("Passed: ralloc algorithm is based on first-fit strategy.\n");
    } else{
        printf("Failed: ralloc algorithm is not based on first-fit strategy.\n");
        failed_test++;
    }
    total_test_executed++;
}

