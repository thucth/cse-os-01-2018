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
			addr_t p_index = page_table->table[i].p_index; // physical page index
			* physical_addr = (p_index << OFFSET_LEN) | (offset);
			return true;
		}
	}
	return false;
}
