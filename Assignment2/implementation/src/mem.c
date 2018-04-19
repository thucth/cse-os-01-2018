#include "mem.h"
#include "stdlib.h"
#include "string.h"
#include <pthread.h>
#include <stdio.h>

static BYTE _ram[RAM_SIZE]; // data on ram, each cell is 1 char

#define flush	fflush(stdout)

/** NUM_PAGES: total page in memory = segments * pages = [segment] + [page] */
static struct {
	uint32_t proc; 	// ID of process currently uses this page
	int index; 		// Index of the page in the list of pages allocated to the process
	int next; 		// The next page in the list. Value -1 if it is the last page
} 
_mem_stat [NUM_PAGES];

static pthread_mutex_t mem_lock;


void init_mem(void) {
	memset(_mem_stat, 0, sizeof(*_mem_stat) * NUM_PAGES);
	memset(_ram, 0, sizeof(BYTE) * RAM_SIZE);
	pthread_mutex_init(&mem_lock, NULL);
}

/** 
 * Get offset of the virtual address
 * 	+ (~0U) << OFFSET_LEN 		= 11..00 (offset times 0)
 * 	+ ~((~0U) << OFFSET_LEN) 	= 00..11 (offset times 1)
 * 	+ addr & 00..11				= offset
 */
static addr_t get_offset(addr_t addr) {
	return addr & ~((~0U) << OFFSET_LEN);
}

/* 
 * Get the first layer index => Get 5 bit Segment index
 * 	+ [address] = [segment] + [page] + [offset]
 * 	+ addr >> (OFFSET_LEN + PAGE_LEN) = [segment]
 */
static addr_t get_first_lv(addr_t addr) {
	return addr >> (OFFSET_LEN + PAGE_LEN);
}

/* 
 * Get the second layer index => Get 5 bit Page index
 * 	+ addr >> OFFSET_LEN 				= [seg] + [page]
 * 	+ get_first_lv(addr) << PAGE_LEN	= [seg] + [0000]
 */
static addr_t get_second_lv(addr_t addr) {
	return (addr >> OFFSET_LEN) - (get_first_lv(addr) << PAGE_LEN);
}

static addr_t get_segment_page_bits(addr_t addr) {
	return addr >> OFFSET_LEN;
}


/** 
 * Search for page table from the a segment table
 * @param: 
 * 		+ index		: segment level index
 * 		+ seg_table	: first level table
 */
static struct page_table_t * get_page_table(addr_t index, struct seg_table_t * seg_table) {
	/**
	 * TODO: Given the Segment index [index], you must go through each
	 * row of the segment table [seg_table] and check if the v_index
	 * field of the row is equal to the index
	 */

	// puts("=== Get_page_table ===");
	// printf("segment_index = %d\n", index); flush;
	// printf("size_segment_table = %d\n", seg_table->size); flush;

	int i;
	for (i = 0; i < seg_table->size; i++) {
		// Enter your code here
		if (seg_table->table[i].v_index == index) {
			// puts("end get_page_table");
			return seg_table->table[i].pages;
		}
		// End code
	}
	// puts("-----ERROR----- get_page_table");
	return NULL;
}

/**
 * Translate virtual address to physical address.
 * If [virtual_addr] is valid, return 1 and write its physical counterpart to [physical_addr].
 * Otherwise, return 0
 * @params:
 * 		+ virtual_addr		: Given virtual address
 * 		+ *physical_addr	: Physical address to be returned
 * 		+ *proc				: Process uses given virtual address
 */
static int translate(addr_t virtual_addr, addr_t * physical_addr, struct pcb_t * proc) {
	// puts("=== Translate ===");
	// printf("Virtual_address = %d\n", virtual_addr); flush;
	// printf("Size_segment_table = %d\n", proc->seg_table->size); flush;

	/* Offset of the virtual address */
	addr_t offset = get_offset(virtual_addr);
	/* The first layer index, find segment virtual */
	addr_t first_lv = get_first_lv(virtual_addr);
	/* The second layer index, find page virtual */
	addr_t second_lv = get_second_lv(virtual_addr);
	
	/* Search in the first level */
	struct page_table_t * page_table = get_page_table(first_lv, proc->seg_table);
	
	if (page_table == NULL) return 0;
	// printf("Size_page_table = %d\n", page_table->size); flush;

	int i;
	for (i = 0; i < page_table->size; i++) {
		// printf("v_index = %d\n", page_table->table[i].v_index); flush;
		if (page_table->table[i].v_index == second_lv) {
			/**
			 * TODO: Concatenate the offset of the virtual addess
			 * to [p_index] field of page_table->table[i] 
			 * to produce the correct physical address 
			 * and save it to [*physical_addr]  
			 */

			addr_t p_index = page_table->table[i].p_index; // physical page index
			* physical_addr = (p_index << OFFSET_LEN) | (offset);
			// printf("Physical_address = %d\n", *physical_addr); flush;
			// puts("end translate");
			// End code
			return 1;
		}
	}
	// puts("-----ERROR----- translate");
	return 0;
}

int memmory_available_to_allocate(int num_pages, struct pcb_t * proc) {
	/**
	 * First we must check if the amount of free memory in virtual address space and 
	 * physical address space is large enough to represent the amount of required memory.
	 * If so, set 1 to [mem_avail].
	 * Hint: check [proc] bit in each page of _mem_stat to know 
	 * whether this page has been used by a process.
	 * For virtual memory space, check bp (break pointer).
	 */

	// Check physical space
	int i = 0;
	int cnt_pages = 0; // count free pages
	for (i = 0; i < NUM_PAGES; i++) {
		if (_mem_stat[i].proc == 0) {
			if (++cnt_pages == num_pages) break;
		}
	}
	if (cnt_pages < num_pages) return 0;

	// Check virtual space
	// addr_t current_bp = proc->bp;
	// addr_t current_segment = get_first_lv(current_bp);
	// struct page_table_t * page_table = get_page_table(current_segment, proc);
	// int segment_available = proc->seg_table->size < max_segment_per_process;
	// if (page_table) {
	// 	int num_used_pages = page_table->size;
	// 	int num_available_pages = max_pages_per_segment - num_used_pages;
	// 	if (num_available_pages >= num_pages) return 1;
	// 	if (!segment_available) return 0;
	// 	// int new_segment = current_segment + 1;
	// 	// proc->bp = new_segment << (OFFSET_LEN + PAGE_LEN);
	// } else {
	// 	if (segment_available) return 1;
	// 	return 0;
	// }

	return 1;
}

void allocate_memory_available(int ret_mem, int num_pages, struct pcb_t * proc) {
	/**
	 * Update status of physical pages which will be allocated to [proc] in _mem_stat. 
	 * Tasks to do:
	 * 	+ Update [proc], [index], and [next] field in _mem_stat
	 * 	+ Add entries to segment table page tables of [proc]
	 * 	  to ensure accesses to allocated memory slot is valid.
	 */

	// puts("= Allocate memory available =");
	int cnt_pages = 0; // count allocated pages
	int last_allocated_page_index = -1; // use for update field [next] of last allocated page
	int i;
	for (i = 0; i < NUM_PAGES; i++) {
		if (_mem_stat[i].proc) continue; // page is used

		_mem_stat[i].proc = proc->pid; // the page is used by process [proc]
		_mem_stat[i].index = cnt_pages; // index in list of allocated pages
		
		if (last_allocated_page_index > -1) { // not initial page, update last page
			_mem_stat[last_allocated_page_index].next = i;
		}
		last_allocated_page_index = i; // update last page

		// Find or Create virtual page table
		addr_t v_address = ret_mem + cnt_pages * PAGE_SIZE; // virtual address of this page
		addr_t v_segment = get_first_lv(v_address);
		struct page_table_t * v_page_table = get_page_table(v_segment, proc->seg_table);
		if (v_page_table == NULL) {
			v_page_table 
				= proc->seg_table->table[proc->seg_table->size++].pages
				= (struct page_table_t*) malloc(sizeof(struct page_table_t));
		}

		int idx = v_page_table->size++;
		v_page_table->table[idx].v_index = get_segment_page_bits(v_address);
		
		addr_t p_address = i << OFFSET_LEN; // physical address of this page
		v_page_table->table[idx].p_index = get_segment_page_bits(p_address);

		// printf("...v_index = %d\n", v_page_table->table[idx].v_index); flush;
		// printf("...p_index = %d\n", v_page_table->table[idx].p_index); flush;

		if (++cnt_pages == num_pages) {
			_mem_stat[i].next = -1; // last page in list
			// printf("Num_pages_reached = %d\n", cnt_pages); flush;
			break;
		}
	}
	// puts("end alloc mem avail");
}

addr_t alloc_mem(uint32_t size, struct pcb_t * proc) {
	// puts("=== Alloc ===");
	// printf("Size_alloc = %d\n", size); flush;
	// printf("Size_seg_table = %d\n", proc->seg_table->size); flush;

	pthread_mutex_lock(&mem_lock);
	addr_t ret_mem = 0;
	/* 
	 * TODO: Allocate [size] byte in the memory for the process [proc] and 
	 * save the address of the first byte in the allocated memory region to [ret_mem].
	 */

	uint32_t num_pages = size / PAGE_SIZE + (size % PAGE_SIZE ? 1 : 0);
	// Number of pages we will use for this process
	// printf("Num_pages_required = %d\n", num_pages);
	
	// memory available? We could allocate new memory region or not?
	int mem_avail = memmory_available_to_allocate(num_pages, proc);
	if (mem_avail) {
		// puts("...memory available");
		/* We could allocate new memory region to the process */
		ret_mem = proc->bp;
		// first byte of new memory required for process
		proc->bp += num_pages * PAGE_SIZE;
		// update break pointer for heap segment process

		allocate_memory_available(ret_mem, num_pages, proc);
		// End code
	}
	pthread_mutex_unlock(&mem_lock);
	// printf("Return_memory = %d\n", ret_mem); flush;
	// puts("end alloc");
	return ret_mem;
}


int free_mem(addr_t address, struct pcb_t * proc) {
	/**
	 * TODO: Release memory region allocated by [proc]. 
	 * The first byte of this region is indicated by [address]. 
	 * Tasks to do:
	 * 	+ Set flag [proc] of physical page use by the memory block back to zero to indicate that it is free.
	 * 	+ Remove unused entries in segment table and page tables of the process [proc].
	 * 	+ Remember to use lock to protect the memory from other processes.
	 */
	
	// puts("=== Free ===");
	// printf("Address = %d\n", address); flush;
	// printf("Size_seg_table = %d\n", proc->seg_table->size); flush;

	pthread_mutex_lock(&mem_lock);

	addr_t v_address = address;	// virtual address to free in process
	addr_t p_address = 0;		// physical address to free in memory
	
	// Find physical page in memory
	if (!translate(v_address, &p_address, proc)) {
		// puts("-----ERROR----- free");
		return 1;
	}

	// Clear physical page in memory
	addr_t p_segment_page_index = get_segment_page_bits(p_address); 
	int cnt_pages = 0; // number of pages in list
	int i = p_segment_page_index;
	for (;;) {
		cnt_pages++;
		_mem_stat[i].proc = 0; // clear physical memory
		i = _mem_stat[i].next; // go to next page in physical memory
		if (i == -1) break; // this page is last
	}

	// Find page table in segment table which is v_index
	addr_t v_segment_page_start = get_segment_page_bits(v_address); // start page in segment
	// printf("v_segment_page_start = %d\n", v_segment_page_start); flush;
	
	for (i = 0; i < cnt_pages; i++) {
		addr_t v_segment_page = v_segment_page_start + i;
		addr_t v_segment = get_first_lv(v_segment_page << OFFSET_LEN);
		struct page_table_t * page_table = get_page_table(v_segment, proc->seg_table);
		int j;
		for (j = 0; j < page_table->size; j++) {
			if (page_table->table[j].v_index == v_segment_page) {
				page_table->table[j].v_index = 0;
				page_table->table[j].p_index = 0;
			}
		}
	}
	pthread_mutex_unlock(&mem_lock);
	// puts("end free");
	// End code
	return 0;
}

int read_mem(addr_t address, struct pcb_t * proc, BYTE * data) {
	// puts("=== Read ===");
	// printf("Virtual_address = %d\n", address); flush;
	addr_t physical_addr;
	if (translate(address, &physical_addr, proc)) {
		// printf("Physical_address = %d\n", physical_addr); flush;
		*data = _ram[physical_addr];
		return 0;
	}
	// puts("end read");
	return 1;
}

int write_mem(addr_t address, struct pcb_t * proc, BYTE data) {
	// puts("=== Write ===");
	// printf("Virtual_address = %d\n", address); flush;
	addr_t physical_addr;
	if (translate(address, &physical_addr, proc)) {
		// printf("Physical_address = %d\n", physical_addr); flush;
		_ram[physical_addr] = data;
		// printf("Write_data = %02x %02x\n", _ram[physical_addr], data); flush;
		return 0;
	}
	// puts("end write");
	return 1;
}

void dump(void) {
	// puts("=== Dump ===");
	int i;
	for (i = 0; i < NUM_PAGES; i++) {
		if (_mem_stat[i].proc != 0) {
			printf("%03d: ", i);
			printf("%05x-%05x - PID: %02d (idx %03d, nxt: %03d)\n",
				i << OFFSET_LEN,
				((i + 1) << OFFSET_LEN) - 1,
				_mem_stat[i].proc,
				_mem_stat[i].index,
				_mem_stat[i].next
			);
			int j;
			for (	j = i << OFFSET_LEN;
				j < ((i+1) << OFFSET_LEN) - 1;
				j++) {
				
				if (_ram[j] != 0) {
					printf("\t%05x: %02x\n", j, _ram[j]);
				}
					
			}
		}
	}
	// puts("end dump");
}


