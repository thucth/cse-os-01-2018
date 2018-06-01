#include "mem.h"
#include "stdlib.h"
#include "string.h"
#include <pthread.h>
#include <stdio.h>

static BYTE _ram[RAM_SIZE]; // data on ram, each cell is 1 char

/** NUM_PAGES: total page in memory = segments * pages = [segment] + [page] */
static struct {
	uint32_t proc; 	// ID of process currently uses this page
	int index; 		// Index of the page in the list of pages allocated to the process
	int next; 		// The next page in the list. Value -1 if it is the last page
} 
_mem_stat [NUM_PAGES];

static pthread_mutex_t mem_lock;
static pthread_mutex_t ram_lock;


void init_mem(void) {
	memset(_mem_stat, 0, sizeof(*_mem_stat) * NUM_PAGES);
	memset(_ram, 0, sizeof(BYTE) * RAM_SIZE);
	pthread_mutex_init(&mem_lock, NULL);
	pthread_mutex_init(&ram_lock, NULL);
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
	if (seg_table == NULL) return NULL;
	int i;
	for (i = 0; i < seg_table->size; i++) {
		if (seg_table->table[i].v_index == index) {
			return seg_table->table[i].pages;
		}
	}
	return NULL;
}

static int remove_page_table(addr_t v_segment, struct seg_table_t * seg_table) {
	if (seg_table == NULL) return 0;
	int i;
	for (i = 0; i < seg_table->size; i++) {
		if (seg_table->table[i].v_index == v_segment) {
			int idx = seg_table->size-1;
			seg_table->table[i] = seg_table->table[idx];
			seg_table->table[idx].v_index = 0;
			free(seg_table->table[idx].pages);
			seg_table->size--;
			return 1;
		}
	}
	return 0;
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
	/* Offset of the virtual address */
	addr_t offset = get_offset(virtual_addr);
	/* The first layer index, find segment virtual */
	addr_t first_lv = get_first_lv(virtual_addr);
	/* The second layer index, find page virtual */
	addr_t second_lv = get_second_lv(virtual_addr);
	
	/* Search in the first level */
	struct page_table_t * page_table = get_page_table(first_lv, proc->seg_table);
	if (page_table == NULL) return false;
	int i;
	for (i = 0; i < page_table->size; i++) {
		if (page_table->table[i].v_index == second_lv) {
			/**
			 * TODO: Concatenate the offset of the virtual addess
			 * to [p_index] field of page_table->table[i] 
			 * to produce the correct physical address 
			 * and save it to [*physical_addr]  
			 */
			addr_t p_index = page_table->table[i].p_index; // physical page index
			* physical_addr = (p_index << OFFSET_LEN) | (offset);
			return true;
		}
	}
	return false;
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
	if (cnt_pages < num_pages) return false;

	// Check virtual space
	if (proc->bp + num_pages*PAGE_SIZE >= RAM_SIZE) return false;

	return true;
}

void allocate_memory_available(int ret_mem, int num_pages, struct pcb_t * proc) {
	/**
	 * Update status of physical pages which will be allocated to [proc] in _mem_stat. 
	 * Tasks to do:
	 * 	+ Update [proc], [index], and [next] field in _mem_stat
	 * 	+ Add entries to segment table page tables of [proc]
	 * 	  to ensure accesses to allocated memory slot is valid.
	 */

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
		// printf("+ pid = %d ::: v_segment = %d, v_page = %d\n", 
		// 	proc->pid, v_segment, get_second_lv(v_address));
		struct page_table_t * v_page_table = get_page_table(v_segment, proc->seg_table);
		if (v_page_table == NULL) {
			int idx = proc->seg_table->size;
			proc->seg_table->table[idx].v_index = v_segment;
			v_page_table
				= proc->seg_table->table[idx].pages
				= (struct page_table_t*) malloc(sizeof(struct page_table_t));
			proc->seg_table->size++;
		}
		int idx = v_page_table->size++;
		v_page_table->table[idx].v_index = get_second_lv(v_address);
		v_page_table->table[idx].p_index = i; // format of i is 10 bit segment and page in address

		if (++cnt_pages == num_pages) {
			_mem_stat[i].next = -1; // last page in list
			break;
		}
	}
}

addr_t alloc_mem(uint32_t size, struct pcb_t * proc) {
	pthread_mutex_lock(&mem_lock);
	addr_t ret_mem = 0;
	/* 
	 * TODO: Allocate [size] byte in the memory for the process [proc] and 
	 * save the address of the first byte in the allocated memory region to [ret_mem].
	 */

	// Number of pages we will use for this process
	uint32_t num_pages = size / PAGE_SIZE + (size % PAGE_SIZE ? 1 : 0);
	
	// memory available? We could allocate new memory region or not?
	int mem_avail = memmory_available_to_allocate(num_pages, proc);
	if (mem_avail) {
		/* We could allocate new memory region to the process */
		ret_mem = proc->bp;
		// first byte of new memory required for process
		proc->bp += num_pages * PAGE_SIZE;
		// update break pointer for heap segment process
		allocate_memory_available(ret_mem, num_pages, proc);
	}

	if (LOG_MEM) {
		puts("============  Allocation  ============");
		dump();
	}

	pthread_mutex_unlock(&mem_lock);
	return ret_mem;
}

void free_mem_break_point(struct pcb_t * proc) {
	while (proc->bp >= PAGE_SIZE) {
		addr_t last_addr = proc->bp - PAGE_SIZE;
		addr_t last_segment = get_first_lv(last_addr);
		addr_t last_page = get_second_lv(last_addr);
		struct page_table_t * page_table = get_page_table(last_segment, proc->seg_table);
		if (page_table == NULL) return;
		while (last_page >= 0) {
			int i;
			for (i = 0; i < page_table->size; i++) {
				if (page_table->table[i].v_index == last_page) {
					proc->bp -= PAGE_SIZE;
					last_page--;
					break;
				}
			}
			if (i == page_table->size) break;
		}
		if (last_page >= 0) break;
	}
}


int free_mem(addr_t address, struct pcb_t * proc) {
	/**
	 * TODO: Release memory region allocated by [proc]. 
	 * The first byte of this region is indicated by [address]. 
	 * Tasks to do:
	 * 	+ Set flag [proc] of physical page use by the memory block 
	 * 	+ back to zero to indicate that it is free.
	 * 	+ Remove unused entries in segment table and page tables of the process [proc].
	 * 	+ Remember to use lock to protect the memory from other processes.
	 */
	pthread_mutex_lock(&mem_lock);
	
	addr_t v_address = address;	// virtual address to free in process
	addr_t p_address = 0;		// physical address to free in memory
	
	// Find physical page in memory
	if (!translate(v_address, &p_address, proc)) return 1;
	
	// Clear physical page in memory
	addr_t p_segment_page_index = p_address >> OFFSET_LEN; 
	int num_pages = 0; // number of pages in list
	int i;
	for (i=p_segment_page_index; i!=-1; i=_mem_stat[i].next) {
		num_pages++;
		_mem_stat[i].proc = 0; // clear physical memory
	}
	
	// Clear virtual page in process
	for (i = 0; i < num_pages; i++) {
		addr_t v_addr = v_address + i * PAGE_SIZE;
		addr_t v_segment = get_first_lv(v_addr);
		addr_t v_page = get_second_lv(v_addr);
		// printf("- pid = %d ::: v_segment = %d, v_page = %d\n", proc->pid, v_segment, v_page);
		struct page_table_t * page_table = get_page_table(v_segment, proc->seg_table);
		if (page_table == NULL) {
			puts("============= Error ===============");
			continue;
		}
		int j;
		for (j = 0; j < page_table->size; j++) {
			if (page_table->table[j].v_index == v_page) {
				int last = --page_table->size;
				page_table->table[j] = page_table->table[last];
				break;
			}
		}
		if (page_table->size == 0) {
			remove_page_table(v_segment, proc->seg_table);
		}
	}
	// Update break pointer
	if (v_address + num_pages * PAGE_SIZE == proc->bp) {
		free_mem_break_point(proc);
	}

	if (LOG_MEM) {
		puts("============  Deallocation  ============");
		dump();
	}

	pthread_mutex_unlock(&mem_lock);
	return 0;
}

int read_mem(addr_t address, struct pcb_t * proc, BYTE * data) {
	addr_t physical_addr;
	if (translate(address, &physical_addr, proc)) {
		pthread_mutex_lock(&ram_lock);
		*data = _ram[physical_addr];
		pthread_mutex_unlock(&ram_lock);
		return 0;
	}
	return 1;
}

int write_mem(addr_t address, struct pcb_t * proc, BYTE data) {
	addr_t physical_addr;
	if (translate(address, &physical_addr, proc)) {
		pthread_mutex_lock(&ram_lock);
		_ram[physical_addr] = data;
		pthread_mutex_unlock(&ram_lock);
		return 0;
	}
	return 1;
}

void dump(void) {
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
			for (j=i<<OFFSET_LEN; j<((i+1)<<OFFSET_LEN)-1; j++) {
				if (_ram[j] != 0) {
					printf("\t%05x: %02x\n", j, _ram[j]);
				}
			}
		}
	}
}