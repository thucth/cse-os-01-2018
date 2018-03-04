#include "procsched.h"
#include <linux/kernel.h>
#include <sys/syscall.h>

#define SIZE 10
#define NUMBER_SYSCALL 377

long procsched(pid_t pid, struct proc_segs * info) {
	long sysvalue;
	unsigned long long args[SIZE];
	sysvalue = syscall(NUMBER_SYSCALL, pid, args);
	if (sysvalue == 0) {
		info->mssv = (unsigned long) args[0];
		info->pcount = (unsigned long) args[1];
		info->run_delay = args[2];
		info->last_arrival = args[3];
		info->last_queued = args[4];
	}
	return sysvalue;
}
