#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>

#define ERROR_ON_SBRK (void*)-1

void * aligned_malloc(unsigned int size, unsigned int align) {

    void *p_cur, *p_res, *p_end;
    int cur, offset, res, end;

    p_cur = sbrk(0); // get current break pointer
    cur = (uintptr_t) p_cur;
    
    // Get offset
    offset = cur % align;
    if (offset > 0) offset = align - offset; 

    // increase offset to align
    if (sbrk(offset) == ERROR_ON_SBRK) {
        printf("Error on sbrk()\n");
        return NULL;
    }

    p_res = sbrk(0); // get current break pointer after offset to align
    res = (uintptr_t) p_res;

    // increase block size
    if (sbrk(size) == ERROR_ON_SBRK) {
        printf("Error on sbrk()\n");
        return NULL;
    }

    p_end = sbrk(0); // get current break pointer after make block
    end = (uintptr_t) p_end;

    printf("offset = %d\n", offset);
    printf("current: %p %d\n", p_cur, cur);
    printf("start  : %p %d\n", p_res, res);
    printf("end    : %p %d\n", p_end, end);

    // return first pointer of block
    return p_res;
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

    printf("size = %d\nalign = %d\n", size, align);

    // malloc array 50 int
    int *a = (int*) aligned_malloc(size, align);

    return 0;
}