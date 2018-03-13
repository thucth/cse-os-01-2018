#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *worker_thread(void *arg)
{
    printf("worker_thread() is created\n");
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

    pthread_exit(NULL);
}