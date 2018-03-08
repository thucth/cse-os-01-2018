#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>

#define ERROR_ON_SBRK (void *)-1

// Store info about block
typedef struct info_block
{
    int size;
    int is_free;
    struct info_block *next;
} info_block;

// Linked list allocated block
static info_block *head, *tail;

info_block *find_free_block(unsigned int size)
{
    info_block *cur = head;

    while (cur)
    {
        if (cur->is_free && cur->size >= size)
            return cur; // free block meet size requirement
        cur = cur->next;
    }

    return NULL;
}

void *advance_malloc(unsigned int size)
{

    info_block *info;
    int total_size;
    void *new_block;

    // find in current list have free block meet size requirement.
    info = find_free_block(size);
    if (info)
    {
        // Found here
        printf("Old block: %lu\n", (uintptr_t)info);
        info->is_free = 0; // not free now
        return (void *)(info + 1);
    }

    // Not found any good free block

    // new block allocated with size must have info_block
    total_size = sizeof(info_block) + size;
    new_block = sbrk(total_size);

    if (new_block == ERROR_ON_SBRK)
        return NULL; // cannot allocate new block

    // store info block
    info = new_block;
    info->size = size;
    info->is_free = 0;
    info->next = NULL;

    // Update state of list via head and tail
    if (!head)
        head = info;
    if (tail)
        tail->next = info;
    tail = info;

    // Return new block
    printf("New block: %lu\n", (uintptr_t)info);

    return (void *)(info + 1);
}

void advance_free(void *block)
{

    info_block *info;
    info_block *tmp;
    void *program_break;

    if (!block)
        return;

    // retrive block info
    info = (info_block *)block - 1;
    // get current program break;
    program_break = sbrk(0);

    // block to free is the last block in list, should update *tail and program break
    if (block + info->size == program_break)
    {
        if (head == tail)
            // no any left blocks
            head = tail = NULL;
        else
        {
            tmp = head;
            // find the block point to end block
            while (tmp)
            {
                if (tmp->next == tail)
                {
                    tmp->next = NULL;
                    tail = tmp;
                }
                tmp = tmp->next;
            }
        }
        // release memory of last block to OS
        sbrk(0 - sizeof(info_block) - info->size);
        printf("release memory to OS\n");
        return;
    }

    // block to free is internal list, only update status free of info_block
    info->is_free = 1;
}

int main()
{

    printf("Initialize program break by printf one line\n");

    int size_2 = 2 * sizeof(int);
    int size_5 = 5 * sizeof(int);
    printf("sizeof(info_block) = %lu\n", sizeof(struct info_block));
    printf("size = %d\n", size_5);

    // malloc [a-b-c-d]
    int *a = (int *)advance_malloc(size_5);
    int *b = (int *)advance_malloc(size_5);
    int *c = (int *)advance_malloc(size_5);
    int *d = (int *)advance_malloc(size_5);

    // free internal [a-b-c@5-d]
    advance_free(c);

    // malloc intenal [a-b-e-d]
    int *e = (int *)advance_malloc(size_2);

    // free internal [a-b@5-e-d]
    advance_free(b);

    // malloc internal [a-f-e-d]
    int *f = (int *)advance_malloc(size_5);

    // free last block [a-f-e]
    advance_free(d);

    // malloc new block [a-f-e-g-h]
    int *g = (int *)advance_malloc(size_2);
    int *h = (int *)advance_malloc(size_5);

    // free small block [a-f-e-g@2-h]
    advance_free(g);

    // malloc large block [a-f-e-g@2-h-i]
    int *i = (int *)advance_malloc(size_5);

    /**
     *********** RESULT ************
    Initialize program break by printf one line
    sizeof(info_block) = 16
    size = 20
    New block: 22835200
    New block: 22835236
    New block: 22835272
    New block: 22835308
    Old block: 22835272
    Old block: 22835236
    release memory to OS
    New block: 22835308
    New block: 22835332
    New block: 22835368
    *********************************
     */

    return 0;
}