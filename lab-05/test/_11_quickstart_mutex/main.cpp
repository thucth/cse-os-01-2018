#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pthread_mutex_t my_mutex;
    pthread_mutexattr_t my_mutex_attr;
    
    // init mutex
    pthread_mutexattr_init(&my_mutex_attr);
    pthread_mutexattr_setpshared(&my_mutex_attr, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&my_mutex, &my_mutex_attr);
    // something here

    // destroy mutex
    pthread_mutexattr_destroy(&my_mutex_attr);
    pthread_mutex_destroy(&my_mutex);
}