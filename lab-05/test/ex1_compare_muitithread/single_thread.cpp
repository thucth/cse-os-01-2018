#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int n = 9999990;
int arr[10000000];

int main()
{
    // Random Array
    for (int i=0;i<n;i++) arr[i] = rand() % 1000;

    int sum = 0;
    for (int i=0;i<n;i++) sum += arr[i];

    printf("%d\n", sum);
}