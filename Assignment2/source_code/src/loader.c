
#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** 
 * Available Process ID
 * used to alloc unique ID for new process 
 */
static uint32_t avail_pid = 1;

#define OPT_CALC	"calc"
#define OPT_ALLOC	"alloc"
#define OPT_FREE	"free"
#define OPT_READ	"read"
#define OPT_WRITE	"write"

static enum ins_opcode_t get_opcode(char * opt) {
	if (!strcmp(opt, OPT_CALC)) {
		return CALC;
	}else if (!strcmp(opt, OPT_ALLOC)) {
		return ALLOC;
	}else if (!strcmp(opt, OPT_FREE)) {
		return FREE;
	}else if (!strcmp(opt, OPT_READ)) {
		return READ;
	}else if (!strcmp(opt, OPT_WRITE)) {
		return WRITE;
	}else{
		printf("Opcode: %s\n", opt);
		exit(1);
	}
}

/**
 * Read infomation of process from /input/proc/*
 */
struct pcb_t * load(const char * path) {
	/* Create new PCB for the new process */
	struct pcb_t * proc = (struct pcb_t * )malloc(sizeof(struct pcb_t));

	proc->pid = avail_pid++;
	proc->seg_table = (struct seg_table_t*)malloc(sizeof(struct seg_table_t));
	proc->bp = PAGE_SIZE;
	proc->pc = 0;

	/* Read process code from file => Open file */
	FILE * file;
	if ((file = fopen(path, "r")) == NULL) {
		printf("Cannot find process description at '%s'\n", path);
		exit(1);
	}

	/** Read priority and number instructions of process */
	char opcode[10];
	proc->code = (struct code_seg_t*)malloc(sizeof(struct code_seg_t));
	fscanf(file, "%u %u", &proc->priority, &proc->code->size);
	proc->code->text = (struct inst_t*)malloc(
		sizeof(struct inst_t) * proc->code->size
	);

	/** Read instructions of process */
	uint32_t i = 0;
	for (i = 0; i < proc->code->size; i++) {
		fscanf(file, "%s", opcode);
		proc->code->text[i].opcode = get_opcode(opcode);
		switch(proc->code->text[i].opcode) {
		case CALC: // not any arguments
			break;
		case ALLOC: // size reg
			fscanf(
				file,
				"%u %u\n",
				&proc->code->text[i].arg_0,
				&proc->code->text[i].arg_1
			);
			break;
		case FREE: // reg
			fscanf(file, "%u\n", &proc->code->text[i].arg_0);
			break;
		case READ:  // source, offset, dest
		case WRITE: // data, dest, offset
			fscanf(
				file,
				"%u %u %u\n",
				&proc->code->text[i].arg_0,
				&proc->code->text[i].arg_1,
				&proc->code->text[i].arg_2
			);
			break;	
		default:
			printf("Opcode: %s\n", opcode);
			exit(1);
		}
	}
	return proc;
}



