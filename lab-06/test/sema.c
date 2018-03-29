#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_TICKETS     35
#define NUM_SELLERS     4
#define true            1
#define false           0

static int numTickets = NUM_TICKETS;
static sem_t ticketLock;

void * sellTicket(void *arg) {
    int done = false;
    int numSoldByThisThread = 0;
    int tid = (int) arg;
    while (!done) {
        sem_wait(&ticketLock);
        if (numTickets == 0) {
            done = true;
        } else {
            numTickets--;
            numSoldByThisThread++;
            printf("Thread %d sold one (%d left)\n", tid, numTickets);
        }
        sem_post(&ticketLock);
    }
    printf("Thread %d sold %d tickets\n", tid, numSoldByThisThread);
    pthread_exit(NULL);
}

int main() {

    int tid[NUM_SELLERS];
    pthread_t sellers[NUM_SELLERS];

    sem_init(&ticketLock, 0, 1);
    int i;
    for (i = 0;i < NUM_SELLERS;i++) {
        tid[i] = i;
        pthread_create(&sellers[i], NULL, sellTicket, tid[i]);
    }

    for (i = 0;i < NUM_SELLERS;i++) {
        pthread_join(sellers[i], NULL);
    }

    sem_destroy(&ticketLock);
    pthread_exit(NULL);

    return 0;
}