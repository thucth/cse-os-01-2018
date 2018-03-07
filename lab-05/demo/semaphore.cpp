#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define MAX 1

int buffer[MAX];
int fill = 0;
int use  = 0;

void put(int value) {
    buffer[fill] = value;     
    fill = (fill + 1) % MAX;  
}

int get() {
    int b = buffer[use];    
    use = (use + 1) % MAX;    
    return b;
}

int loops = 0;

sem_t empty;
sem_t full;
sem_t mutex; 

void *producer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        //sem_wait(&empty);
        sem_wait(&mutex); 
        put(i);
        sem_post(&mutex); 
        //sem_post(&full);
    }
}

void *consumer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        //sem_wait(&full);
        sem_wait(&mutex); 
        int b = get();
        sem_post(&mutex);
        //sem_post(&empty);
        printf("%d\n", b);
    }
}

int main(int argc, char *argv[])
{

    loops = 10;

    sem_init(&empty, 0, MAX); // MAX buffers are empty to begin with...
    sem_init(&full, 0, 0);    // ... and 0 are full
    sem_init(&mutex, 0, 1);    // mutex = 1 since it a lock

    pthread_t pThread, cThread;
    pthread_create(&pThread, 0, producer, 0);
    pthread_create(&cThread, 0, consumer, 0);
    pthread_join(pThread, NULL);
    pthread_join(cThread, NULL);
    return 0;
}