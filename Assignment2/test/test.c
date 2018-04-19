#include <stdio.h>

struct A {
    int a;
    char * c;
};

int main() {
    struct A * a;
    struct A * b = a;
    a->a = 5;
    b->c = "hello";

    printf("%d\n", b->a);
    puts(a->c);
}