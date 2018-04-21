#include "cpu.h"
#include "timer.h"
#include "sched.h"
#include "loader.h"
#include "mem.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ulong unsigned long

static int time_slot;
static int num_cpus;
static int done = 0;

int num_processes;

// arguments in thread used for loader routine
static struct loader_args_t {
	char ** path; // path to input file for each process
	ulong * start_time; // time process start
} 
	loader_processes;

// arguments in thread used for cpu routine
struct cpu_args_t {
	struct timer_id_t * timer_id;
	int id; // cpu id, not process id
};

static void * cpu_routine(void * args) {
	struct cpu_args_t * cpu_args = (struct cpu_args_t*) args;
	struct timer_id_t * timer_id = cpu_args->timer_id;
	int id = cpu_args->id;

	/* Check for new process in ready queue */
	int time_left = 0;
	struct pcb_t * proc = NULL;
	while (1) {
		/* Check the status of current process */
		if (proc == NULL) {
			/* No process is running, the we load new process from ready queue */
			proc = get_proc();
		} else if (proc->pc == proc->code->size) {
			/* The porcess has finish it job */
			printf("\tCPU %d: Processed %2d has finished\n", id, proc->pid);
			free(proc);
			proc = get_proc(); // get next process
			time_left = 0;
		} else if (time_left == 0) {
			/* The process has done its job in current time slot */
			printf("\tCPU %d: Put process %2d to run queue\n", id, proc->pid);
			put_proc(proc);
			proc = get_proc(); // get next process
		}
		
		/* Recheck process status after loading new process */
		if (proc == NULL && done) {
			/* No process to run, exit */
			printf("\tCPU %d stopped\n", id);
			break;
		} else if (proc == NULL) {
			/* There may be new processes to run in
			 * next time slots, just skip current slot */
			next_slot(timer_id);
			continue;
		} else if (time_left == 0) {
			printf("\tCPU %d: Dispatched process %2d\n", id, proc->pid);
			time_left = time_slot;
		}
		
		/* Run current process */
		run(proc);
		time_left--;
		next_slot(timer_id);
	}
	detach_event(timer_id);
	pthread_exit(NULL);
}

static void * loader_routine(void * args) {
	struct timer_id_t * timer_id = (struct timer_id_t*) args;
	int i = 0;
	while (i < num_processes) {
		struct pcb_t * proc = load(loader_processes.path[i]);
		while (current_time() < loader_processes.start_time[i]) {
			next_slot(timer_id);
		}
		printf("\tLoaded a process at %s, PID: %d\n", 
			loader_processes.path[i], proc->pid);
		add_proc(proc);
		free(loader_processes.path[i++]);
		next_slot(timer_id);
	}
	free(loader_processes.path);
	free(loader_processes.start_time);
	done = 1;
	detach_event(timer_id);
	pthread_exit(NULL);
}

static void read_config(const char * path) {
	FILE * file;
	if ((file = fopen(path, "r")) == NULL) {
		printf("Cannot find configure file at %s\n", path);
		exit(1);
	}
	fscanf(file, "%d %d %d\n", &time_slot, &num_cpus, &num_processes);
	loader_processes.path = (char**) malloc(sizeof(char*) * num_processes);
	loader_processes.start_time = (ulong*) malloc(sizeof(ulong) * num_processes);

	int i;
	for (i = 0; i < num_processes; i++) {
		loader_processes.path[i] = (char*) malloc(sizeof(char) * 100);
		loader_processes.path[i][0] = '\0';
		strcat(loader_processes.path[i], "input/proc/");
		char proc[100];
		fscanf(file, "%lu %s\n", &loader_processes.start_time[i], proc);
		strcat(loader_processes.path[i], proc);
	}
}


int main(int argc, char * argv[]) {
	
	/* Read config */
	if (argc != 2) {
		printf("Usage: os [path to configure file]\n");
		return 1;
	}

	char path[100];
	path[0] = '\0';
	strcat(path, "input/");
	strcat(path, argv[1]);
	read_config(path);

	pthread_t * cpu = (pthread_t*) malloc(num_cpus * sizeof(pthread_t));
	struct cpu_args_t * cpu_args = 
		(struct cpu_args_t*) malloc(sizeof(struct cpu_args_t) * num_cpus);

	pthread_t loader;
	
	/* Init timer */
	int i;
	for (i = 0; i < num_cpus; i++) {
		cpu_args[i].timer_id = attach_event();
		cpu_args[i].id = i;
	}
	struct timer_id_t * loader_event = attach_event();
	start_timer();

	/* Init scheduler */
	init_scheduler();

	/* Run CPU and loader */
	pthread_create(&loader, NULL, loader_routine, (void*) loader_event);
	for (i = 0; i < num_cpus; i++) {
		pthread_create(&cpu[i], NULL, cpu_routine, (void*) &cpu_args[i]);
	}

	/* Wait for CPU and loader finishing */
	for (i = 0; i < num_cpus; i++) {
		pthread_join(cpu[i], NULL);
	}
	pthread_join(loader, NULL);

	/* Stop timer */
	stop_timer();

	printf("\nMEMORY CONTENT:\n");
	dump();

	return 0;

}