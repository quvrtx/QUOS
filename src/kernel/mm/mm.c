#include "mm/buddy.h"
#include <kernel/mm/mm.h>
#include <lib/stddef.h>

buddy_alloc_t* buddy;

void init_allocator(void* start, size_t size){
    buddy = (buddy_alloc_t*)start;
    init_buddy_alloc(buddy, (void*)((uintptr_t)start + (uintptr_t)sizeof(buddy_alloc_t)), size-sizeof(buddy_alloc_t));
}

void* kmalloc(size_t size){
    return alloc_buddy_alloc(buddy, size);
}