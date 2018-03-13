#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *worker_thread(void *arg)
{
    printf("worker_thread() is created\n");
    int lval = 100000;
    int rval = lval + 500;
    int mval = (lval + rval) / 2;
    for (int i = lval; i < rval; i++)
    {
        printf("Thread 2: %d\n", i);
        usleep(5000);
        if (i == mval)
        {
            // exit pthread
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t my_thread;
    int ret;

    printf("main(): create new thread\n");
    ret = pthread_create(&my_thread, NULL, &worker_thread, NULL);

    if (ret != 0)
    {
        printf("error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 100; i++)
    {
        printf("Thread 1: %d\n", i);
        usleep(8000);
    }

    pthread_exit(NULL);
}