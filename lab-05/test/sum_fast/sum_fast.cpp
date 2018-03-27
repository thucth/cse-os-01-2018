#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define EL puts("")

typedef pair<int,int> info_thread;
#define left_index first
#define right_index second

const int N = 1e8+5;
int n, a[N], f[N], sum;

pthread_t thread[N];

void * worker_thread(void * arg) {
    info_thread * ith = (info_thread *) arg;
    // printf("%d %d\n", ith->first, ith->second);
    f[ith->left_index] += f[ith->right_index];
    pthread_exit(NULL);
}

void sum_by_thread() {
    for (int len = n; len > 1; len = (len+1)/2) {
        // printf("-----------%d\n", len);
        for (int i = 0; i < len/2; i++) {
            info_thread *ith = new info_thread;
            ith->left_index = i;
            ith->right_index = len-i-1;
            pthread_create(&thread[i], NULL, worker_thread, ith);
        }
        for (int i = 0; i < len/2; i++) {
            pthread_join(thread[i], NULL);
        }
        // printf("===========%d\n", len);
    }
    printf("Sum by thread = %d\n", f[0]);
}

int main(int narg, char ** args) {

    srand(time(NULL));

    if (narg == 1) {
        return puts("Error: n is required");
    }

    n = atoi(args[1]);
    if (n <= 0) {
        return puts("Error: n <= 0 || n is not number");
    }

    printf("narg = %d\n", narg);
    printf("arg[1] = %s\n", args[1]);
    printf("n = %d\n", n);

    for (int i = 0; i < n; i++) {
        f[i] = a[i] = rand() % 100;
        sum += a[i]; // calculate sum first
        // printf("%d %d\n", f[i], a[i]);
        // printf("%d ", a[i]);
    }
    printf("Sum by normal = %d\n", sum);

    // Call sum using multithread
    sum_by_thread();

    return 0;
}