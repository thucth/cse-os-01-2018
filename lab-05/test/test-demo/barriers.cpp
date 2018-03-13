#include <pthread.h>
#include <stdio.h>

pthread_barrier_t b;

void *task(void* param)
{
    long id = (long)param;
    printf("before the barrier %ld\n", id);
    pthread_barrier_wait(&b); // line 1
    printf("after the barrier %ld\n", id);
}

int main()
{
    int nThread = 5;
    int i;

    pthread_t thread[nThread];
    
    pthread_barrier_init(&b, 0, nThread);
    
    for(i = 0; i < nThread; i++)
        pthread_create(&thread[i], 0, task, (void *)i);
    
    for(i = 0; i < nThread; i++)
        pthread_join(thread[i], 0);
    
    pthread_barrier_destroy(&b);

    /** if have line 1:

        before the barrier 1
        before the barrier 2
        before the barrier 0
        before the barrier 3
        before the barrier 4
        after the barrier 1
        after the barrier 4
        after the barrier 2
        after the barrier 3
        after the barrier 0

        ========================================

        if comment line 1:
        
        before the barrier 1
        after the barrier 1
        before the barrier 0
        after the barrier 0
        before the barrier 2
        after the barrier 2
        before the barrier 3
        after the barrier 3
        before the barrier 4
        after the barrier 4

     */

    return 0;
}