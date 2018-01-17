#include <stdio.h>
#include "./sum.h"
#include "./sub.h"

void main()
{
    int a = 0, b = 0;

    printf("Nhap hai so:\n");
    
    printf("a = ");
    scanf("%d", &a);
    
    printf("b = ");
    scanf("%d", &b);

    printf("sum = %d\n", sum(a, b));
    printf("sub = %d\n", sub(a, b));
}