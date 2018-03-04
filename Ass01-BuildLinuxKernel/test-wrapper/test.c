#include "procsched.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
	struct proc_segs *p;
	long sysvalue;
	sysvalue = procsched(1, p);
	printf("Sysvalue = %ld\n", sysvalue);
	if (sysvalue == 0) {
		printf("Found:\n");	
		printf("mssv = %lu\n", p->mssv);
		printf("pcount = %lu\n", p->pcount);
		printf("run_delay = %llu\n", p->run_delay);
		printf("arrival = %llu\n", p->last_arrival);
		printf("queued = %llu\n", p->last_queued);
	}
	else {
		printf("Not found:\n");
	}
}
