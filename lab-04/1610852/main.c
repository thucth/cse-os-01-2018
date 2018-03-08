#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ex1.h"

int power(int x, int m)
{
    if (m == 0)
        return 1;
    int tmp = power(x, m / 2);
    if (m % 2 == 0)
        return tmp * tmp;
    return tmp * tmp * x;
}

int main()
{

    printf("Initialize program break by printf one line\n");

    int align = power(2, 20);
    int size_2 = 2 * sizeof(int);
    int size_5 = 5 * sizeof(int);

    printf("sizeof(info_block) = %lu\n", sizeof(info_block));
    printf("size_5 = %d\n", size_5);
    printf("size_2 = %d\n", size_2);
    printf("align  = %d\n", align);

    // malloc [a-b-c-d]
    int *a = (int *)aligned_malloc(size_5, align);
    int *b = (int *)aligned_malloc(size_5, align);
    int *c = (int *)aligned_malloc(size_5, align);
    int *d = (int *)aligned_malloc(size_5, align);

    // free internal [a-b-c@5-d]
    aligned_free(c);

    // malloc intenal [a-b-e-d]
    int *e = (int *)aligned_malloc(size_2, align);

    // free internal [a-b@5-e-d]
    aligned_free(b);

    // malloc internal [a-f-e-d]
    int *f = (int *)aligned_malloc(size_5, align);

    // free last block [a-f-e]
    aligned_free(d);

    // malloc new block [a-f-e-g-h]
    int *g = (int *)aligned_malloc(size_2, align);
    int *h = (int *)aligned_malloc(size_5, align);

    // free small block [a-f-e-g@2-h]
    aligned_free(g);

    // malloc large block [a-f-e-g@2-h-i]
    int *i = (int *)aligned_malloc(size_5, align);

    // free block g is free [a-f-e-g-h-i]
    aligned_free(g);

    // free all
    aligned_free(a);
    aligned_free(f);
    aligned_free(e);
    aligned_free(h);
    aligned_free(i);

    /************* RESULT ************
    Initialize program break by printf one line
    sizeof(info_block) = 16
    size_5 = 20
    size_2 = 8
    align  = 1048576
    New block: Info at 31457264 - Block at 31457280
    New block: Info at 32505840 - Block at 32505856
    New block: Info at 33554416 - Block at 33554432
    New block: Info at 34602992 - Block at 34603008
    Old block: Info at 33554416 - Block at 33554432
    Old block: Info at 32505840 - Block at 32505856
    Release memory to OS
    New block: Info at 34602992 - Block at 34603008
    New block: Info at 35651568 - Block at 35651584
    New block: Info at 36700144 - Block at 36700160
    Release memory to OS
    **********************************/

    return 0;
}