#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>

#define ERROR_ON_SBRK (void*)-1

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

void *allocate_aligned_block(unsigned int size, unsigned int align) 
{
    void *p_cur, *p_block, *p_info_block;
    int cur, offset;

    // allocate block for info block
    if (sbrk(sizeof(info_block)) == ERROR_ON_SBRK) 
        return NULL;

    p_cur = sbrk(0);
    cur = (uintptr_t) p_cur;
    
    // Get offset
    offset = cur % align;
    if (offset > 0) offset = align - offset; 

    // increase offset to align
    if (sbrk(offset) == ERROR_ON_SBRK)
        return NULL;

    // get pointer to block
    p_block = sbrk(0);

    // increase block size
    if (sbrk(size) == ERROR_ON_SBRK)
        return NULL;

    // info block is behind block
    p_info_block = p_block - sizeof(info_block);

    return p_info_block;
}

void *advance_malloc(unsigned int size, unsigned int align)
{
    void *return_block;
    info_block *info;
    void *new_block;

    // find in current list have free block meet size requirement.
    info = find_free_block(size);
    if (info)
    {
        // Found here
        info->is_free = 0; // not free now
        return_block = (void *)(info + 1);
        printf("Old block: Info at %lu - Block at %lu\n", (uintptr_t)info, (uintptr_t)return_block);
        return return_block;
    }

    // Not found any good free block

    // new block allocated with size must have info_block
    new_block = allocate_aligned_block(size, align);

    if (new_block == NULL)
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
    return_block = (void *)(info + 1);
    printf("New block: Info at %lu - Block at %lu\n", (uintptr_t)info, (uintptr_t)return_block);
    return return_block;
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
        printf("Release memory to OS\n");
        return;
    }

    // block to free is internal list, only update status free of info_block
    info->is_free = 1;
}

int power(int x, int m) {
    if (m == 0) return 1;
    int tmp = power(x, m/2);
    if (m % 2 == 0) return tmp * tmp;
    return tmp * tmp * x; 
}

int main() {

    printf("Initialize program break by printf one line\n");

    int align = power(2, 20);
    int size_2 = 2 * sizeof(int);
    int size_5 = 5 * sizeof(int);
    
    printf("sizeof(info_block) = %lu\n", sizeof(struct info_block));
    printf("size_5 = %d\n", size_5);
    printf("size_2 = %d\n", size_2);
    printf("align  = %d\n", align);

    // malloc [a-b-c-d]
    int *a = (int *)advance_malloc(size_5, align);
    int *b = (int *)advance_malloc(size_5, align);
    int *c = (int *)advance_malloc(size_5, align);
    int *d = (int *)advance_malloc(size_5, align);

    // free internal [a-b-c@5-d]
    advance_free(c);

    // malloc intenal [a-b-e-d]
    int *e = (int *)advance_malloc(size_2, align);

    // free internal [a-b@5-e-d]
    advance_free(b);

    // malloc internal [a-f-e-d]
    int *f = (int *)advance_malloc(size_5, align);

    // free last block [a-f-e]
    advance_free(d);

    // malloc new block [a-f-e-g-h]
    int *g = (int *)advance_malloc(size_2, align);
    int *h = (int *)advance_malloc(size_5, align);

    // free small block [a-f-e-g@2-h]
    advance_free(g);

    // malloc large block [a-f-e-g@2-h-i]
    int *i = (int *)advance_malloc(size_5, align);

    // free block g is free [a-f-e-g-h-i]
    advance_free(g);

    // free all
    advance_free(a);
    advance_free(f);
    advance_free(e);
    advance_free(h);
    advance_free(i);

    /************* RESULT ************
    Initialize program break by printf one line
    sizeof(info_block) = 16
    size_5 = 20
    size_2 = 8
    align  = 1048576
    New block: Info at 36700144 - Block at 36700160
    New block: Info at 37748720 - Block at 37748736
    New block: Info at 38797296 - Block at 38797312
    New block: Info at 39845872 - Block at 39845888
    Old block: Info at 38797296 - Block at 38797312
    Old block: Info at 37748720 - Block at 37748736
    Release memory to OS
    New block: Info at 39845872 - Block at 39845888
    New block: Info at 40894448 - Block at 40894464
    New block: Info at 41943024 - Block at 41943040
    Release memory to OS
    **********************************/
    
    return 0;
}