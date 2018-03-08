#ifndef ALIGNED_MALLOC_FREE_SYSCALL
#define ALIGNED_MALLOC_FREE_SYSCALL

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>

// Store info about block
typedef struct info_block
{
    int size;
    int is_free;
    struct info_block *next;
} info_block;

// Linked list allocated block
static info_block *head, *tail;

void * aligned_malloc(unsigned int size, unsigned int align);

void * aligned_free(void * ptr);

#endif