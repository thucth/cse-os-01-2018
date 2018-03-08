#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    
    // sbrk(0);
    // sbrk(5);
    // sbrk(0);

    // printf("something here\n");

    // int t = 7;

    int * a = malloc(50 * sizeof(int));

    printf("1. 0: %lu\n", (uintptr_t) sbrk(0));
    printf("2. 0: %lu\n", (uintptr_t) sbrk(0));
    printf("3. 5: %lu\n", (uintptr_t) sbrk(5));
    printf("4. 0: %lu\n", (uintptr_t) sbrk(0));
    printf("5. 0: %lu\n", (uintptr_t) sbrk(0));

    int * b = malloc(50 * sizeof(int));
    printf("6. 0: %lu\n", (uintptr_t) sbrk(0));

    return 0;
}