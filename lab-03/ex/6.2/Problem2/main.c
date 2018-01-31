#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int createProcess(char parent, char child) {
    int newProcess = fork();
    if (newProcess > 0) { // this is real create, parent create child
        printf("Process %c (%d) create process %c (%d)\n", parent, getpid(), child, newProcess);
        return 1;
    }
    return 0;
}

int main() {

    int AB = createProcess('A', 'B');
    if (AB) {
        int AC = createProcess('A', 'C');
        if (AC) {
            int AD = createProcess('A', 'D');
        } else { // this is process C
            int CG = createProcess('C', 'G');
        }
    }
    else { // this is process B
        int BE = createProcess('B', 'E');
            if (BE) {
                int BF = createProcess('B', 'F');
            } else {
                int EI = createProcess('E', 'I');
            }
    }

    return 0;
}