#ifndef BUDDY_H
#define BUDDY_H

#include <lib/stddef.h> // for size_t
#include <lib/stdint.h> // for i8, u8, i16...

#define MAX_ORDER 12 // 4KB (1 page)

/* free block in buddy allocator */
typedef struct free_block{
    i32 order: 7;                    // 2^n = size of free block (n = order)
    i32 used: 1;                     // used or not
    struct free_block* next;         // element of linked list in entity of buddy allocator
} free_block_t;

/* entity of buddy allocator */
typedef struct buddy_alloc{  
    void* start;                     // start of memory of entity of buddy allocator
    size_t size;                     // size of avilaible memory in entity of buddy allocator
    free_block_t free[MAX_ORDER];    // linked list for each of sizes
} buddy_alloc_t;

int init_buddy_alloc(buddy_alloc_t* alloc, void* start, size_t size); // create inittial block and fill buddy_alloc_t structure
void* alloc_buddy_alloc(buddy_alloc_t* alloc, size_t size);                 // allocate block with aligned size
int free_buddy_alloc(buddy_alloc_t* alloc, void* ptr);                // free block by ptr

#endif //BUDDY_H