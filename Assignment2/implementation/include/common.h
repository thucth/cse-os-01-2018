#ifndef COMMON_H
#define COMMON_H

/* Define structs and routine could be used by every source files */

#include <stdint.h>

#define ADDRESS_SIZE	20
#define OFFSET_LEN		10
#define SEGMENT_LEN		5
#define PAGE_LEN		5

#define NUM_PAGES		(1 << (ADDRESS_SIZE - OFFSET_LEN))
#define PAGE_SIZE		(1 << OFFSET_LEN)
/**
 * Use 20 bit to represent the address.
 * the first 5 bits for segment index, 
 * the next 5 bits for page index, 
 * the last 10 bits for offset
 * 
 * [address](20) = [segment](5) + [page](5) + [offset](10)
 */

#define true			1
#define false			0
#define flush			fflush(stdout)
#define min(a, b)		((a) < (b) ? (a) : (b))
#define max(a, b)		((a) > (b) ? (a) : (b))


typedef char BYTE;
typedef uint32_t addr_t;

/** instructions opcode type */
enum ins_opcode_t {
	CALC,	// Just perform calculation, only use CPU
	ALLOC,	// Allocate memory
	FREE,	// Deallocated a memory block
	READ,	// Write data to a byte on memory
	WRITE	// Read data from a byte on memory
};

/* instructions executed by the CPU */
struct inst_t {
	enum ins_opcode_t opcode;
	// Argument lists for instructions
	uint32_t arg_0; 
	uint32_t arg_1;
	uint32_t arg_2;
	/**
	 * Arguments for each opcode:
	 * calc		
	 * alloc	size 	reg
	 * free		reg
	 * read		source 	offset 	dest
	 * write	data 	dest 	offset
	 */
};

struct code_seg_t {
	struct inst_t * text;
	uint32_t size;
};

struct page_table_t {
	/* A row in the page table of the second layer */
	struct  {
		addr_t v_index; // The index of virtual address,  [page] bits
		addr_t p_index; // The index of physical address, [segment]+[page] bits
	} table[1 << PAGE_LEN];
	int size;
};

/* Mapping virtual addresses and physical ones */
struct seg_table_t {
	/* Translation table for the first layer */
	struct {
		addr_t v_index;	// Virtual index, only [segment] bits
		struct page_table_t * pages;
	} table[1 << SEGMENT_LEN];
	int size;	// Number of row in the first layer
};

/* PCB, describe information about a process */
struct pcb_t {
	uint32_t pid;
	uint32_t priority;
	struct code_seg_t * code;
	addr_t regs[10]; 
	// Registers, store address of allocated regions
	uint32_t pc; 
	// Program counter, point to the next instruction
	struct seg_table_t * seg_table; 
	// Page table, used to translate virtual addresses to physical addresses
	uint32_t bp;
	// Break pointer, used to manage the heap segment.
};

#endif