#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREAD 1000

typedef struct {
    int id;
    int nPoints; // IN - number points random each thread
    // int count;   // OUT - count points in circle each thread
} InfoThread;

// InfoThread ith[MAX_THREAD];

int nThreads;
int nPoints;
int counter = 0;

pthread_mutex_t lock;

float my_rand() {
    return -1 + ((float) rand() / (float) RAND_MAX) * 2;
}

void *func_thread(void *arg) {
    InfoThread *ith = (InfoThread *)arg;
    // printf("Thread %d is created: nPoints = %d\n", ith->id, ith->nPoints);

    for (int i = 0; i < ith->nPoints; i++) {
        float x = my_rand();
        float y = my_rand();
        // ith->count += (x * x + y * y <= 1);
        pthread_mutex_lock(&lock);
        counter += (x * x + y * y <= 1);
        pthread_mutex_unlock(&lock);
    }

    // printf("Exit thread %d: count = %d\n", ith->id, ith->count);
    pthread_exit(NULL);
}

int main(int nargs, char *args[])
{
    // srand(time(NULL));

    // printf("nargs = %d\n", nargs);
    // printf("arg[1] = %s\n", args[1]);
    
    if (nargs < 2) {
        printf("Require number points\n");
        return 0;
    }

    nPoints = atoi(args[1]);
    // printf("nPoints = %d\n", nPoints);
    // printf("===============================================\n");

    if (nPoints < 0) {
        printf("Number points < 0\n");
        return 0;
    }

    pthread_t my_threads[MAX_THREAD];
  
    // Change nThreads to test
    // nThreads = 1;
    // nThreads = 2;
    // nThreads = 4;
    nThreads = 8;
    // nThreads = 16;
    // nThreads = 10;
    // nThreads = 100;

    printf("nThreads = %d\n", nThreads);

    pthread_mutex_init(&lock, NULL);

    int left = nPoints % nThreads;
    for (int i = 0; i < nThreads; i++) {
        InfoThread ith;
        ith.id = i;
        ith.nPoints = nPoints / nThreads + (i < left);
        pthread_create(&my_threads[i], NULL, func_thread, &ith);
    }

    // wait for all thread is completed
    for (int i = 0; i < nThreads; i++) {
        pthread_join(my_threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    // printf("================================================\n");
    // printf("All thread is completed\n");

    // printf("Points in circle = %d\n", cnt);

    float pi = 4 * (float) counter / (float) nPoints;

    printf("PI = %.12f\n", pi);

    return 0;
}
