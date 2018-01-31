#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int value = 5;

int main() {

    // PROCESS ID - FORK()

    // switch (fork()) {
    //     case 0: 
    //         printf("child: pid=%d\n", getpid());
    //         break;
    //     case -1:
    //         perror("Fork error");
    //         break;
    //     default:
    //         printf("parent: pid=%d\n", getpid());
    // }

    // int a = 0;
    // for (int i=0;i<3;i++) {
    //     int id = fork();
    //     printf("i = %2d - pid = %6d - id_fork = %6d - a = %2d\n", i, getpid(), id, ++a);
    // }

    /*
    i =  0 - pid =  16534 - id_fork =  16535 - a =  1
    i =  0 - pid =  16535 - id_fork =      0 - a =  1
    i =  1 - pid =  16534 - id_fork =  16536 - a =  2
    i =  1 - pid =  16536 - id_fork =      0 - a =  2
    i =  2 - pid =  16534 - id_fork =  16537 - a =  3
    i =  1 - pid =  16535 - id_fork =  16538 - a =  2
    i =  2 - pid =  16537 - id_fork =      0 - a =  3
    i =  2 - pid =  16536 - id_fork =  16539 - a =  3
    i =  2 - pid =  16535 - id_fork =  16540 - a =  3
    i =  1 - pid =  16538 - id_fork =      0 - a =  2
    i =  2 - pid =  16540 - id_fork =      0 - a =  3
    i =  2 - pid =  16538 - id_fork =  16541 - a =  3
    i =  2 - pid =  16541 - id_fork =      0 - a =  3
    i =  2 - pid =  16539 - id_fork =      0 - a =  3
    */


    // SLEEP

    // int timestamp = 0;
    // for (;;) {
    //     printf("Time: %5d\n", timestamp++);
    //     sleep(1);
    // }

    // int a = 10;
    // if (fork() == 0) {
    //     a++;
    // } else {
    //     wait();
    //     printf("%d\n", a);
    // }

    // int a = fork();
    // if (a != 0) {
    //     int b = fork();
    // }
    // printf("%d\n", getpid());

    


    return 0;
}