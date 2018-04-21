#include "mem.h"

#define flush fflush(stdout)

int mem_init(uint size) {
    pthread_mutex_init(&lock, NULL);
    mem_pool = (void *) malloc(sizeof(void) * size);
    break_point = mem_pool;
    limit_pool_size = size;
    return mem_pool != 0;
}

void free_mem_region(struct mem_region * region) {
    while (region) {
        struct mem_region * tmp = used_regions;
        region = region->prev;
        free(tmp);
    }
}

void mem_finish() {
    free_mem_region(used_regions);
    free_mem_region(free_regions);
    free(mem_pool);
}

void * mem_alloc(uint size) {
    pthread_mutex_lock(&lock);
    void * pointer = best_fit_allocator(size);
    if (pointer) {
        printf("Alloc [%4d bytes] %p - %p\n", size, pointer, (char *) pointer + size -1);
        flush;
    } else {
        printf("Alloc [%4d bytes] NULL\n", size);
        flush;
    }
    pthread_mutex_unlock(&lock);
    return pointer;
}

void mem_free(void * pointer) {
    pthread_mutex_lock(&lock);
    struct mem_region * target = used_regions;
    struct mem_region * next = NULL;
    while (target) {
        if (target->pointer == pointer) {
            break;
        }
        target = target->prev;
        next = target;
    }
    if (!target) {
        pthread_mutex_unlock(&lock);
        return;
    }
    if (next) {
        next->prev = target->prev;
        if (next->prev) {
            next->prev->next = next;
        }
    } else {
        used_regions = NULL;
    }
    target->prev = free_regions;
    target->next = NULL;
    if (free_regions) {
       free_regions->next = target;
    }
    free_regions = target;
    pthread_mutex_unlock(&lock);
}

void * best_fit_allocator(uint size) {
    struct mem_region * walker = free_regions;
    struct mem_region * best = NULL;
    while (walker) {
        if (walker->size >= size && (best == NULL || best->size > walker->size)) {
            best = walker;
        }
        walker = walker->prev;
    }
    if (best) return (void *) best->pointer;
    if (break_point + size > mem_pool + limit_pool_size) {
        return NULL;
    }
    best = (struct mem_region *) malloc(sizeof(struct mem_region));
    best->size = size;
    best->pointer = break_point;
    break_point += size;
    best->prev = used_regions;
    best->next = NULL;
    if (used_regions) {
        used_regions->next = best;
    }
    used_regions = best;

    return (void *) best->pointer;
}