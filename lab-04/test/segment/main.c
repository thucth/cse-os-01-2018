#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int ga = 99;
int gb;

void func() {
    int la = 9;
    printf("pid=%d\n", getpid());
    printf("ga:%p\n", &ga);
    printf("gb:%p\n", &gb);
    printf("func:%p\n", &func);
    printf("la:%p\n", &la);
}

int main() {
    func();
    return 0;
}