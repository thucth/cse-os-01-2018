#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "findsubstr.h"
#include "readline.h"

int main(int argc, char *argv[])
{
    // printf("%d\n", argc);
    // for (int i=0;i<argc;i++) printf("%s\n", argv[i]);

    // int pos = find_sub_string("import React from 'react';", "react");
    // printf("%d\n", pos);


    char * grep;
    if (argc == 1) puts("Not have grep");
    else strcpy(grep, argv[1]);

    // printf("%s\n", grep);

    int bufsize = 1024;
    int lines = 0, cnt = 0;

    // char *s = "asdfasdf";
    // printf("%s\n", s);

    for (;;) {
        char * str = (char *)malloc(bufsize * sizeof(char));
        cnt = read_line(str);
        if (cnt < 0) break;
        // printf("Line %2d + Size: %3d + Content:%s", ++lines, cnt, str);
        int pos = find_sub_string(str, grep);
        // printf("%d\n", pos);
        if (pos > -1) {
            // printf("*Match*: position match: %3d\n", pos);
            printf("%s", str);
        }
    }
    puts("\n");

    return 0;
}