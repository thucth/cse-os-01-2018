#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "findsubstr.h"
// #include "readline.h"

int main(int argc, char *argv[]) {
    // printf("%d\n", argc);
    // for (int i=0;i<argc;i++) printf("%s\n", argv[i]);

    // int t = find_sub_string('he he he', 'h');
    // printf("$d\n", t);

    // if (argc == 1) {
    //     puts("File not found");
    //     return 1;
    // }

    // char * fileName = argv[1];
    
    char * fileName = "inp.inp";
    
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(fileName, "r");
    
    if (fp == NULL) {
        puts("File not found");
        return 0;
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);
    }

    fclose(fp);

    return 0;
}