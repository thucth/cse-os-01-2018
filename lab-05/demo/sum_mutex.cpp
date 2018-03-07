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
pthread_mutex_t lock;

void *Sum(void *i) {
   long k = (long)i;
   //pthread_mutex_lock(&lock);
   sum = sum + k;
   //pthread_mutex_unlock(&lock);
}

int main () {
   pthread_t threads[NUM_THREADS];
   int rc;
   int i;
   
   if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
   
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
   pthread_mutex_destroy(&lock);
   sum = 0;
   for (i = 0; i < NUM_THREADS; i++)
   {
       sum = sum + i;
   }
   cout << "Sum must be:" << sum << endl;
   return 0;
}