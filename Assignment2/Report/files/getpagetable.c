static struct page_table_t * get_page_table(addr_t index, struct seg_table_t * seg_table) {

	if (seg_table == NULL) return NULL;

	int i;
	for (i = 0; i < seg_table->size; i++) {
		if (seg_table->table[i].v_index == index) {
			return seg_table->table[i].pages;
		}
	}
	return NULL;
}
