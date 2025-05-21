#include <mm/buddy.h>

static size_t align_up(size_t size){
    if (size == 0) return 0;
    size_t aligned_size = 1;

    while (aligned_size < size){
        aligned_size <<= 1;

        if (aligned_size == 0) return 0;
    }
    return aligned_size;
}

static free_block* get_buddy(BuddyAlloc* alloc, free_block* block, int order) {
    uintptr_t buddy_addr = (uintptr_t)block - (uintptr_t)alloc->heap_start;
    buddy_addr ^= (1 << order);
    return (free_block*)(buddy_addr + (uintptr_t)alloc->heap_start);
}
static int split(BuddyAlloc* alloc, int order){
    if (!alloc->free_list[order].next) return 1;

    free_block* block1 = alloc->free_list[order].next;
    free_block* block2 = get_buddy(alloc, block1, order-1);

    block1->next = block2;
    block2->next = alloc->free_list[order-1].next;

    block1->header.used = 0;
    block1->header.order = order-1;
    block2->header.used = 0;
    block2->header.order = order-1;

    alloc->free_list[order].next = alloc->free_list[order].next->next;
    alloc->free_list[order-1].next = block1;

    return 0;
}

static int merge(BuddyAlloc* alloc, int order){
    
}

void init_buddy(BuddyAlloc *alloc, void *heap_start, size_t heap_size){
    int total_size = sizeof(header_block) + heap_size;

    alloc->heap_start = heap_start;
    alloc->heap_size = heap_size;

    for (int i = 0; i <= MAX_ORDER; i++){
        alloc->free_list[i].next = NULL;
    }

    int order = 0;
    while ((1 << order) < heap_size && order <= MAX_ORDER) order++;

    free_block* init_block = (free_block*)heap_start;
    init_block->next = NULL;
    init_block->header.order = order;
    init_block->header.used = 0;

    alloc->free_list[order].next = init_block;
}

void* alloc_buddy(BuddyAlloc* alloc, size_t size){
    if (!alloc || !size || alloc->heap_size < align_up(size)) return NULL;

    size_t total_size = sizeof(header_block) + size;
    total_size = align_up(total_size);

    size_t order = 0;
    while ((1 << order) < total_size && order <= MAX_ORDER) order++;

    if (order >= MAX_ORDER) return NULL;

    int higher_order = order;
    while ((1 << higher_order) < total_size && order <= MAX_ORDER) higher_order++;

    while (higher_order > order){
        if (split(alloc, higher_order)) return NULL;
        higher_order--;
    }

    alloc->free_list[order].next->header.used = 1;
    alloc->free_list[order].next->header.order = order;

    free_block* res = alloc->free_list[order].next;
    alloc->free_list[order].next = res->next;

    return (void*)((uintptr_t)res + sizeof(header_block));
}

void free_buddy(BuddyAlloc *alloc, void *ptr){
    free_block* block = (free_block*)((uintptr_t)ptr - sizeof(header_block));

    size_t order = block->header.order;
    
    if (!alloc || block->header.used || !ptr) return;

    block->header.used = 1;

    while (order < MAX_ORDER){
        free_block* block2 = get_buddy(alloc, block, order);

        int can_merge = 1;

        if ((uintptr_t)block2+(1 << order) < ((uintptr_t)alloc->heap_start + (uintptr_t)alloc->heap_size) || 
            (uintptr_t)block2 > (uintptr_t)alloc->heap_start) can_merge = 0;

        if (can_merge){
            free_block* current = alloc->free_list[order].next;
            while (current){
                if (current == block2 && current->header.used == 0 && current->header.order == order) break;
            }
            if (!current) can_merge = 0;
        }

        if (!can_merge) break;
    }
}