#include <sys/syscall.h>
#include <stdio.h>
#define SIZE 10

int main() {
	long sysvalue;
	unsigned long info[SIZE];
	sysvalue = syscall(377, 1, info);
	int i;
	for (i=0;i<5;i++) {
		printf("%lu\n", info[i]);
	}
}
