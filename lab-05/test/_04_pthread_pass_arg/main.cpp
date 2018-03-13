#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct worker_arg {
    int time;
    int lval;
    int rval;
};

void *worker_thread(void *arg)
{
    printf("worker_thread() is created\n");
    worker_arg *targ = (worker_arg *) arg;
    int lval = targ->lval;
    int rval = targ->rval;
    int time = targ->time;
    printf("%d\n%d\n%d\n", lval, rval, time);
    for (int i = lval; i < rval; i++)
    {
        printf("Thread 2: %d\n", i);
        usleep(5000);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t my_thread;
    worker_arg *arg;
    int ret;

    arg->lval = 1000000;
    arg->rval = arg->lval + 50;
    arg->time = 5000;

    printf("main(): create new thread\n");
    ret = pthread_create(&my_thread, NULL, &worker_thread, arg);

    if (ret != 0)
    {
        printf("error\n");
        exit(EXIT_FAILURE);
    }

    pthread_exit(NULL);
}