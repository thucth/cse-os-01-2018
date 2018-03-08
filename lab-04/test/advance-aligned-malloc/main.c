#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>

#define ERROR_ON_SBRK (void*)-1

// Store info about block
typedef struct info_block {
    int size;
    int is_free;
    struct info_block *next;
} info_block;

// Linked list allocated block
info_block *head, *tail;

void * aligned_malloc(unsigned int size, unsigned int align) {

    info_block *cur;
    info_block *info;
    int total_size;
    void *new_block;
    unsigned long address;

    // find in current list have free block meet size requirement.
    cur = head;

    while (cur) {
        if (cur->is_free && cur->size >= size) {
            cur->is_free = 0; // not free now
            printf("Found Block\n");
            address = (uintptr_t) cur;
            printf("%lu\n", address);
            return (void *)(cur+1); // free block meet size requirement
        }
        cur = cur->next;
    }

    // Not found any good free block
    
    // new block allocated with size must have info_block
    total_size = sizeof(info_block) + size;
    new_block = sbrk(total_size);

    if (new_block == ERROR_ON_SBRK) {
        return NULL; // cannot allocate new block
    }

    // store info block
    info = new_block;
    info->size = size;
    info->is_free = 0;
    info->next = NULL;

    // Update state of list via head and tail
    if (!head) {
        head = info;
    }
    if (tail) {
        tail->next = info;
    }
    tail = info;

    // Return new block
    printf("New Block\n");
    address = (uintptr_t) info;
    printf("%lu\n", address);
    return (void *)(info + 1);

    // void *p_cur, *p_res, *p_end;
    // int cur, offset, res, end;

    // p_cur = sbrk(0); // get current break pointer
    // cur = (uintptr_t) p_cur;
    
    // // Get offset
    // offset = cur % align;
    // if (offset > 0) offset = align - offset; 

    // // increase offset to align
    // if (sbrk(offset) == ERROR_ON_SBRK) {
    //     printf("Error on sbrk()\n");
    //     return NULL;
    // }

    // p_res = sbrk(0); // get current break pointer after offset to align
    // res = (uintptr_t) p_res;

    // // increase block size
    // if (sbrk(size) == ERROR_ON_SBRK) {
    //     printf("Error on sbrk()\n");
    //     return NULL;
    // }

    // p_end = sbrk(0); // get current break pointer after make block
    // end = (uintptr_t) p_end;

    // printf("offset = %d\n", offset);
    // printf("current: %p %d\n", p_cur, cur);
    // printf("start  : %p %d\n", p_res, res);
    // printf("end    : %p %d\n", p_end, end);

    // // return first pointer of block
    // return p_res;
}

int power(int x, int m) {
    if (m == 0) return 1;
    int tmp = power(x, m/2);
    if (m % 2 == 0) return tmp * tmp;
    return tmp * tmp * x; 
}

int main() {

    int align = power(2, 25);
    int size = 50 * sizeof(int);

    // printf("size = %d\nalign = %d\n", size, align);

    // malloc array 50 int
    int *a = (int*) aligned_malloc(size, align);

    return 0;
}