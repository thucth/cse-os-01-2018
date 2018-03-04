#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ex1.h"

int power(int x, int m) {
    if (m == 0) return 1;
    int tmp = power(x, m/2);
    if (m % 2 == 0) return tmp * tmp;
    return tmp * tmp * x; 
}

int main() {

    printf("Main:\n");

    int align = power(2, 15);
    int size = 50 * sizeof(int);

    // malloc array 50 int
    int *a = (int*) aligned_malloc(size, align);

    // free memory
    aligned_free(a);

    return 0;
}