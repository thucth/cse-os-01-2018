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

...
	// Clear virtual page in process
	for (i = 0; i < num_pages; i++) {
		addr_t v_addr = v_address + i * PAGE_SIZE;
		addr_t v_segment = get_first_lv(v_addr);
		addr_t v_page = get_second_lv(v_addr);
		
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
...
