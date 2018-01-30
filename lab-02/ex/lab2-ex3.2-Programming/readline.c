#include "readline.h"

int read_line(char *str)
{
    size_t bufsize = 1024, count = 0;

    char *buffer = (char*) malloc(bufsize * sizeof(char));

    if (buffer == NULL) {
        puts("Not allocate buffer");
        exit(1);
    }

    count = getline(&buffer, &bufsize, stdin);

    strcpy(str, buffer);
    
    return count;
}