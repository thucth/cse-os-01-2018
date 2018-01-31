#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int val = 5;

int main() {


    int pid = fork();
    if(pid==0) {
        val+=15;return 0;
    }
    else if (pid>0){
        wait(NULL);
        printf("Partent: value=%d", val );
        return 0;
    }

    return 0;

}