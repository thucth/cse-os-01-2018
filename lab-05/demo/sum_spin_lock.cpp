#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define NUM_THREADS 1000

volatile int sum = 0;
pthread_spinlock_t slock;

void *Sum(void *i) {
   long k = (long)i;
   pthread_spin_lock(&slock);
   sum = sum + k;
   pthread_spin_unlock(&slock);
}

int main () {
   pthread_t threads[NUM_THREADS];
   int rc;
   int i;
   
    pthread_spin_init(&slock, PTHREAD_PROCESS_PRIVATE);
   
   for( i = 0; i < NUM_THREADS; i++ ) {
      rc = pthread_create(&threads[i], NULL, Sum, (void *)i);
      
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   for (i = 0; i < NUM_THREADS; i++)
   {
       pthread_join(threads[i], NULL);
   }
   cout << "Final, Sum is:" << sum << endl;
   pthread_spin_destroy(&slock);
   sum = 0;
   for (i = 0; i < NUM_THREADS; i++)
   {
       sum = sum + i;
   }
   cout << "Sum must be:" << sum << endl;
   return 0;
}