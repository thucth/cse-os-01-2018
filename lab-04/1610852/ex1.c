#include "ex1.h"

#define ERROR_ON_SBRK (void*)-1

void * aligned_malloc(unsigned int size, unsigned int align) {

    if (size == 0) {
        return NULL;
    }

    void *p_cur, *p_res, *p_end;
    int cur, offset, res, end;

    p_cur = sbrk(0); // get current break pointer
    cur = (uintptr_t) p_cur; // get decimal value address
    
    // Get offset which need to go to multiple of align
    offset = align - cur % align;

    // increase offset to align
    if (sbrk(offset) == ERROR_ON_SBRK) {
        printf("Error on sbrk()\n");
        return NULL;
    }

    p_res = sbrk(0); // get current break pointer after offset to align
    res = (uintptr_t) p_res; // address in decimal

    // increase block size bytes
    if (sbrk(size) == ERROR_ON_SBRK) {
        printf("Error on sbrk()\n");
        return NULL;
    }

    p_end = sbrk(0); // get current break pointer after make block
    end = (uintptr_t) p_end;

    // store info block (size and offset) to free
    int * info = p_res-1;
    info[0] = offset + size;

    printf("Malloc:\n");
    printf("size   = %d\n", size);
    printf("align  = %d\n", align);
    printf("offset = %d\n", offset);
    printf("info   = %d\n", info[0]);
    printf("current: %p %d\n", p_cur, cur);
    printf("start  : %p %d\n", p_res, res);
    printf("end    : %p %d\n", p_end, end);

    // return first pointer of block, which after offset
    return p_res;
}

void * aligned_free(void * ptr) {

    if (ptr == NULL) {
        return NULL;
    }
    
    // retrive info block (size and offset)
    int * info = ptr-1;
    int size = info[0];

    // get current pointer
    void *p_cur = sbrk(0);
    int cur = (uintptr_t) p_cur; // address in decimal

    // free block by descrease size bytes
    if (sbrk(-size) == ERROR_ON_SBRK) {
        printf("Error on sbrk()\n");
        return NULL;
    }

    // get current pointer after free
    void *p_end = sbrk(0);
    int end = (uintptr_t) p_end;

    printf("Free:\n");
    printf("size   = %d\n", size);
    printf("current: %p %d\n", p_cur, cur);
    printf("end    : %p %d\n", p_end, end);

    return p_end;
}