void allocate_memory_available(int ret_mem, int num_pages, struct pcb_t * proc) {

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
