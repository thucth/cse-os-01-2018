#include <linux/module.h> // included for all kernel modules
#include <linux/kernel.h> // included for KERN_INFO
#include <linux/init.h>   // included for __init and __exit macros
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/pid.h>

struct pid *pid_struct;
struct task_struct *task;
struct sched_info si;
static int pid = 1231; 	

static int __init procsched_init(void)
{
	printk(KERN_INFO "Starting kernel module!\n");
	
	pid_struct = find_get_pid(pid);
	task = pid_task(pid_struct, PIDTYPE_PID);

	if (task) {
		printk("INFO TASK\n");
		printk("state = %ld\n", task->state);
		printk("comm  = %s\n", task->comm);
		si = task->sched_info;
		printk("pcount  = %lu\n", si.pcount);
		printk("delay   = %llu\n", si.run_delay);
		printk("arrival = %llu\n", si.last_arrival);
		printk("queued  = %llu\n", si.last_queued);
	}
	else {
		printk("LOG: task not found with pid");	
	}

	return 0;
}

static void __exit procsched_cleanup(void)
{
	printk(KERN_INFO "Cleaning up module\n");
}

MODULE_LICENSE("GPL");
module_init(procsched_init);
module_exit(procsched_cleanup);
module_param(pid, int, 0);
