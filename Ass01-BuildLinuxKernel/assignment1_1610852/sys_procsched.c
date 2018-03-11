#include <linux/linkage.h>
#include <linux/sched.h>
#include <linux/pid.h>

struct proc_segs {
	unsigned long mssv;
	unsigned long pcount;
	unsigned long long run_delay;
	unsigned long long last_arrival;
	unsigned long long last_queued;
};

asmlinkage long sys_procsched(int pid, struct proc_segs * info) {
	/// TODO: Implement syscall
	
	struct pid *pid_struct;
	struct task_struct *task;
	struct sched_info si;

	pid_struct = find_get_pid(pid);
	task = pid_task(pid_struct, PIDTYPE_PID);
	
	info->mssv = 1610852;

	if (task) {
		// Found task
		si = task->sched_info;
		info->pcount = si.pcount;
		info->run_delay = si.run_delay;
		info->last_arrival = si.last_arrival;
		info->last_queued = si.last_queued;

		return 0;
	}
	else {
		// Not found task with pid
		return -1;
	}
}
