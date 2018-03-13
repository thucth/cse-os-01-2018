#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *worker_thread_1(void *arg)
{
    printf("worker_thread_1() is created\n");
    int lval = 100000;
    int rval = lval + 50;
    for (int i = lval; i < rval; i++)
    {
        printf("Thread 1: %d\n", i);
        usleep(4000);
    }
    pthread_exit(NULL);
}

void *worker_thread_2(void *arg)
{
    printf("worker_thread_2() is created\n");
    int lval = 900000;
    int rval = lval + 50;
    for (int i = lval; i < rval; i++)
    {
        printf("Thread 2: %d\n", i);
        usleep(5000);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t thread_1, thread_2;
    int ret_1, ret_2;

    printf("main(): create thread 1\n");
    ret_1 = pthread_create(&thread_1, NULL, &worker_thread_1, NULL);

    if (ret_1 != 0)
    {
        printf("error\n");
        exit(EXIT_FAILURE);
    }

    printf("main(): create thread 2\n");
    ret_2 = pthread_create(&thread_2, NULL, &worker_thread_2, NULL);

    if (ret_2 != 0)
    {
        printf("error\n");
        exit(EXIT_FAILURE);
    }

    // waiting until thread 1 and 2 exit
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    for (int i = 0; i < 10; i++)
    {
        printf("Thread 0: %d\n", i);
        usleep(6000);
    }

    pthread_exit(NULL);
}