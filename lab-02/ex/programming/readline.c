#include "readline.h"

int read_line(char *str)
{
    char *buffer;
    size_t bufsize = 1024;
    size_t count;

    buffer = (char *)malloc(bufsize * sizeof(char));

    if (buffer == NULL)
    {
        puts("Unable to allocate buffer");
        exit(1);
    }

    count = getline(&buffer, &bufsize, stdin);

    strcpy(str, buffer);

    return count;
}