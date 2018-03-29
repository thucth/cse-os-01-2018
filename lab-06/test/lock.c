#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

static volatile int counter = 0;
pthread_mutex_t lock;

void *myThread(void *arg) {
    printf("%s\n", (char *)arg);
    
    // comment line (1) and (2) to test with no sync

    pthread_mutex_lock(&lock); // (1)
    int i;
    for (i = 0; i < 1e7; i++) {
        counter = counter + 1;
    }
    pthread_mutex_unlock(&lock); // (2)

    printf("%s\n", (char *) arg);
    return NULL;
}

int main() {

    pthread_t p1, p2;
    int rc;

    pthread_mutex_init(&lock, NULL);
    
    printf("main: begin (counter - %d)\n", counter);

    rc = pthread_create(&p1, NULL, myThread, "A"); 
    assert(rc == 0);

    rc = pthread_create(&p2, NULL, myThread, "B"); 
    assert(rc == 0);

    rc = pthread_join(p1, NULL); assert(rc == 0);
    rc = pthread_join(p2, NULL); assert(rc == 0);

    printf("main: finish (counter - %d)\n", counter);

    return 0;
}