#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "mem.h"

#define ARRAY_SIZE 10

void * proc(void *args) {
    int i;
    int index = 0;
    char * mem[ARRAY_SIZE];
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (rand() % 2) {
            uint size = 1 << ((rand() % 4) + 4);
            mem[index] = mem_alloc(size);
            if (mem[index]) ++index;
        } else {
            if (index == 0) continue;
            char j = rand() % index;
            mem_free(mem[j]);
            mem[j] = mem[--index];
        }
    }
}

int main() {

    srand(time(NULL));

    mem_init(1 << 10);

    pthread_t p0, p1;
    pthread_create(&p0, NULL, proc , NULL);
    pthread_create(&p1, NULL, proc , NULL);
    pthread_join(p0, NULL);
    pthread_join(p1, NULL);

    mem_finish();

    return 0;
}