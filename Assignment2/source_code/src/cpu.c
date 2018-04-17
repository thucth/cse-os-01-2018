
#include "cpu.h"
#include "mem.h"

static int calc(struct pcb_t * proc) {
	return ((unsigned long)proc & 0UL);
}

/* Allocate [size] memory for process [proc] on register [reg_index] */
static int alloc(struct pcb_t * proc, uint32_t size, uint32_t reg_index) {
	addr_t addr = alloc_mem(size, proc);
	if (addr == 0) {
		return 1;
	}else{
		// map value address for register on process
		proc->regs[reg_index] = addr;
		return 0;
	}
}

/* Deallocate memory at register [reg_index] of process [proc] */
static int free_data(struct pcb_t * proc, uint32_t reg_index) {
	return free_mem(proc->regs[reg_index], proc);
}

/* 
 * Read a byte at address which equal to value of register [source + offset]
 * and save to register [destination]
 */
static int read(
		struct pcb_t * proc, // Process executing the instruction
		uint32_t source, // Index of source register
		uint32_t offset, // Source address = [source] + [offset]
		uint32_t destination // Index of destination register
) { 
	
	BYTE data;
	if (read_mem(proc->regs[source] + offset, proc,	&data)) {
		proc->regs[destination] = data;
		return 0;
	}else{
		return 1;
	}
}

/**
 * Write data to register
 */
static int write(
		struct pcb_t * proc, // Process executing the instruction
		BYTE data, // Data to be wrttien into memory
		uint32_t destination, // Index of destination register
		uint32_t offset // Destination address = [destination] + [offset]
) { 	
	return write_mem(proc->regs[destination] + offset, proc, data);
}

int run(struct pcb_t * proc) {
	/* Check if Program Counter point to the proper instruction */
	if (proc->pc >= proc->code->size) {
		return 1;
	}
	
	struct inst_t ins = proc->code->text[proc->pc];
	proc->pc++;
	int stat = 1;
	switch (ins.opcode) {
	case CALC:
		stat = calc(proc);
		break;
	case ALLOC:
		stat = alloc(proc, ins.arg_0, ins.arg_1);
		break;
	case FREE:
		stat = free_data(proc, ins.arg_0);
		break;
	case READ:
		stat = read(proc, ins.arg_0, ins.arg_1, ins.arg_2);
		break;
	case WRITE:
		stat = write(proc, ins.arg_0, ins.arg_1, ins.arg_2);
		break;
	default:
		stat = 1;
	}
	return stat;

}


