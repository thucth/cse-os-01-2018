struct pcb_t * get_proc(void) {
	struct pcb_t * proc = NULL;

	pthread_mutex_lock(&queue_lock);
	if (empty(&ready_queue)) {
		// move all process is waiting in run_queue back to ready_queue
		while (!empty(&run_queue)) {
			enqueue(&ready_queue, dequeue(&run_queue));
		}
	}

	if (!empty(&ready_queue)) {
		proc = dequeue(&ready_queue);
	}
	pthread_mutex_unlock(&queue_lock);

	return proc;
}
