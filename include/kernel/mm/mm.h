#ifndef MM_H
#define MM_H

#include <kernel/mm/buddy.h>

void init_allocator(void* start, size_t size);
void* kmalloc(size_t size);

#endif //MM_H