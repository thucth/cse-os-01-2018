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

...	
	// Update break pointer
	addr_t v_segment_page = v_address >> OFFSET_LEN;
	if (v_segment_page + num_pages * PAGE_SIZE == proc->bp) {
		free_mem_break_point(proc);
	}
...
