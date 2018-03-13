#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int n = 9999990;
int arr[10000000];
int nblock = 4;

void *th_work(void *arg) {
    int * res = (int *) arg;
    int block = *res;
    int unit = n / nblock;
    int l = unit * block;
    int r = l + unit - 1;
    if (r >= n) r = n-1;

    // printf("%d %d %d\n", block, l, r);
    for (int i=l;i<r;i++) *res = *res + i;

    pthread_exit(NULL);
}

void sum_use_thread() {
    pthread_t th[nblock];
    int res[nblock];
    for (int i=0;i<nblock;i++) {
        res[i] = i;
        pthread_create(&th[i], NULL, &th_work, &res[i]);;
    }
    for (int i=0;i<nblock;i++) pthread_join(th[i], NULL);
    int sum = 0;
    for (int i=0;i<nblock;i++) {
        // printf("%d\n", res[i]);
        sum += res[i];
    }
    printf("%d\n", sum);
}

int main()
{
    // Random Array
    for (int i=0;i<n;i++) arr[i] = rand() % 1000;

    // for (int i=0;i<n;i++) printf("%d\n", arr[i]);
    // printf("============================================\n");

    sum_use_thread();
}