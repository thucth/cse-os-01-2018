#ifndef MEMORY_MANAGEMENT
#define MEMORY_MANAGEMENT

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef unsigned int uint;

struct mem_region {
    size_t size;
    char * pointer;
    struct mem_region * next;
    struct mem_region * prev;
};

static void * mem_pool = NULL;
static void * break_point = NULL;
static int limit_pool_size = 0;

static struct mem_region * free_regions = NULL;
static struct mem_region * used_regions = NULL;

pthread_mutex_t lock;

int mem_init(uint size);
void mem_finish();
void * mem_alloc(uint size);
void mem_free(void * pointer);
void * best_fit_allocator(uint size);

#endif