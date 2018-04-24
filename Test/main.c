#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    int a, e;

    a = 10;
    if (fork() == 0) {
        a = a*2;
        if (fork() == 0) {
            a++;
            exit(2);
        }
        printf("%d\n", a);
        exit(1);
    }
    wait(&e);

    printf("a: %d, e: %d\n", a, WEXITSTATUS(e));

    return 0;
}