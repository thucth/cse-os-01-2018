#ifndef _PROC_SCHED_H_
#define _PROC_SCHED_H_

#include <unistd.h>

struct proc_segs {
	unsigned long mssv;
	unsigned long pcount;
	unsigned long long run_delay;
	unsigned long long last_arrival;
	unsigned long long last_queued;
};

long procsched(pid_t pid, struct proc_segs * info);

#endif
