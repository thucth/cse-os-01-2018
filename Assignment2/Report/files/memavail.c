int memmory_available_to_allocate(int num_pages, struct pcb_t * proc) {
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
