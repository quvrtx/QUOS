#ifndef BUDDY_H
#define BUDDY_H

#include <lib/stddef.h>
#include <lib/stdint.h>

#define MAX_ORDER 12

typedef struct header_block{
    uint8_t order;
    int used: 1;
} header_block;

typedef struct free_block{
    header_block header;
    struct free_block* next;
} free_block;

typedef struct {
    void* heap_start;
    size_t heap_size;
    free_block free_list[MAX_ORDER + 1];
} BuddyAlloc;

void init_buddy(BuddyAlloc* alloc, void* heap_start, size_t heap_size);
void* alloc_buddy(BuddyAlloc* alloc, size_t size);
void free_buddy(BuddyAlloc* alloc, void* ptr);

#endif //BUDDY_H