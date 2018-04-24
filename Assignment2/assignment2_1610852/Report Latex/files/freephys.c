...
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
...
