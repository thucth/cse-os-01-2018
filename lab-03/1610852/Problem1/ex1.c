#include <stdio.h>
#include <unistd.h>

int main() {

    int a[1000], n = 0;
    
    FILE *fi = fopen("./numbers.txt", "r");

    if (fi == NULL) {
        puts("File not found");
        return 0;
    }

    int tmp;
    while (fscanf(fi, "%d", &tmp) == 1) {
        a[n++] = tmp;
    }
    fclose(fi);

    // printf("%d\n", n);
    // for (int i=0;i<n;i++) printf("%d\n", a[i]);

    if (fork() == 0) { // child, div 3
        int cnt = 0;
        for (int i=0;i<n;i++) cnt += (a[i] % 3 == 0) ? 1 : 0;
        printf("%d\n", cnt);
    } else { // parent, div 2
        int cnt = 0;
        for (int i=0;i<n;i++) cnt += (a[i] %2 == 0) ? 1 : 0;
        printf("%d\n", cnt);
    }

    return 0;
}
