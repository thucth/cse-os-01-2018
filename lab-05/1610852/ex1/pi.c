#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_THREAD 61028
// #define MAX_THREAD 30000
// #define MAX_THREAD 10000
// #define MAX_THREAD 8192

typedef struct {
    int id;
    int nPoints; // IN - number points random each thread
    int count;   // OUT - count points in circle each thread
} InfoThread;


int nThreads = MAX_THREAD;
int nPoints;


double my_rand() {
    return -1.0 + (1.0 * rand() / RAND_MAX) * 2.0;
}

void *func_thread(void *arg) {
    InfoThread *ith = (InfoThread *)arg;
    // printf("Thread %d is created: nPoints = %d\n", ith->id, ith->nPoints);

    for (int i = 0; i < ith->nPoints; i++) {
        double x = my_rand();
        double y = my_rand();
        ith->count += (sqrt(x * x + y * y) <= 1);
    }

    // printf("Exit thread %d: count = %d\n", ith->id, ith->count);
    pthread_exit(NULL);
}

int main(int nargs, char *args[])
{
    srand(time(NULL));

    // printf("nargs = %d\n", nargs);
    // printf("arg[1] = %s\n", args[1]);

    nPoints = atoi(args[1]);
    // printf("nPoints = %d\n", nPoints);
    // printf("===============================================\n");

    pthread_t my_threads[MAX_THREAD];
    InfoThread ith[MAX_THREAD];

    // Change nThreads to test
    // nThreads = 1;
    // nThreads = 2;
    // nThreads = 4;
    // nThreads = 8;
    // nThreads = 16;
    nThreads = (int) sqrt(nPoints);

    printf("nThreads = %d\n", nThreads);

    for (int i = 0; i < nThreads; i++) {
        ith[i].id = i;
        ith[i].count = 0;
        ith[i].nPoints = nPoints / nThreads + (i < nPoints % nThreads);
        pthread_create(&my_threads[i], NULL, &func_thread, &ith[i]);
    }

    // wait for all thread is completed
    for (int i = 0; i < nThreads; i++) {
        pthread_join(my_threads[i], NULL);
    }

    // printf("================================================\n");
    // printf("All thread is completed\n");

    int cnt = 0;
    for (int i = 0; i < nThreads; i++) {
        cnt += ith[i].count;
    }

    // printf("Points in circle = %d\n", cnt);

    double pi = 4.0 * cnt / (double)nPoints;

    printf("PI = %.9f\n", pi);

    return 0;
}
