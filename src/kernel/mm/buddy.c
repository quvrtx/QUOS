#include <mm/buddy.h>
#include <lib/stdint.h>


/* align to order of 2 */
static size_t align(size_t size){
    if (!size) return 0;
    int i = 0;
    for (i = 0; (1 << i) < size; i++){}
    return (1 << i);
}

/* XOR finding buddy */
static free_block_t* get_buddy(buddy_alloc_t* alloc, free_block_t* block1, int order){
    free_block_t* block2 = (free_block_t*)(((uintptr_t)block1 - (uintptr_t)alloc->start) ^ (1 << order));
    block2 = (free_block_t*)((uintptr_t)block2 + (uintptr_t)alloc->start);
    return block2;
}

/* remove block from free list */
static void remove_block_from_free_list(buddy_alloc_t* alloc, free_block_t* block){
    for (int i = 0; i <= MAX_ORDER; i++){
        free_block_t* current_block = alloc->free[i].next;

        if (current_block == block){
            alloc->free[i].next = current_block->next;
            return;
        }

        while (current_block != NULL){
            if (current_block->next == block){
                current_block->next = current_block->next->next;
                return;
            }

            current_block = current_block->next;
        }
    }
}

/* remove block from free list by order */
static void remove_block_from_free_list_by_order(buddy_alloc_t* alloc, free_block_t* block, int order){
    free_block_t* current_block = alloc->free[order].next;

    if (current_block == block){
            alloc->free[order].next = current_block->next;
            return;
        }

    while (current_block != NULL){
        if (current_block->next == block){
            current_block->next = current_block->next->next;
            return;
        }

        current_block = current_block->next;
    }
}

/* split block to 2 blocks */
static int split(buddy_alloc_t* alloc, free_block_t* block1){
    // Errors 
    if (!alloc || !block1 || !(
        (uintptr_t)alloc->start < (uintptr_t)block1 &&
        (uintptr_t)alloc->start + (uintptr_t)alloc->size > (uintptr_t)block1 + (uintptr_t)sizeof(free_block_t)
    )) return -1;

    int order = block1->order - 1;

    free_block_t* block2 = get_buddy(alloc, block1, order);

    // Filling block1
    block1->next = block2;
    block1->used = 0;

    // Filling block2
    block2->order = order;
    block2->used = 0;
    block2->next = alloc->free[order+1].next;

    // Update free list
    alloc->free[order].next = block1;
    alloc->free[order+1].next = alloc->free[order+1].next->next;

    return 0;
}

/* merge block1 and block2 (founded by get_buddy func) to 1 block */
static int merge(buddy_alloc_t* alloc, free_block_t* block1){
    // Errors
    if (!alloc || !block1 || !(
        (uintptr_t)alloc->start < (uintptr_t)block1 &&
        (uintptr_t)alloc->start + (uintptr_t)alloc->size > (uintptr_t)block1 + (uintptr_t)sizeof(free_block_t)
    )) return -1;

    int order = block1->order + 1;

    free_block_t* block2 = get_buddy(alloc, block1, order - 1);

    remove_block_from_free_list_by_order(alloc, block1, order-1);
    remove_block_from_free_list_by_order(alloc, block2, order-1);

    block1->order = order;
    block1->next = alloc->free[order].next;

    alloc->free[order].next = block1;

    return 0;
}

int init_buddy_alloc(buddy_alloc_t *alloc, void *start, size_t size){
    if (!alloc || !start || !size){
        return -1;
    }

    alloc->start = start;
    alloc->size = size;
    
    for (int i = 0; i <= MAX_ORDER; i++){
        alloc->free[i].next = NULL;
    }

    int init_order = 0;
    while (init_order < MAX_ORDER){
        if (!((1 << init_order) < size)){
            init_order--;
            break;
        }
    }

    free_block_t* init_block = (free_block_t*)start;
    init_block->order = init_order;
    init_block->used = 0;
    init_block->next = NULL;

    alloc->free[init_order].next = init_block;

    return 0;
}

void* alloc_buddy_alloc(buddy_alloc_t* alloc, size_t size){
    if (!alloc || !size || alloc->size < size){
        return NULL;
    }
    size_t total_size = align(sizeof(free_block_t)+size);

    int need_order = 0;
    for (need_order = 0; (1 << need_order) < total_size; need_order++){}

    int free_order;
    for (free_order = MAX_ORDER; alloc->free[free_order].next != NULL && free_order >= 0 && free_order >= need_order; free_order--){}

    while (free_order > need_order){
        split(alloc, alloc->free[free_order].next);

        free_order--;
    }

    free_block_t* ret_block = alloc->free[need_order].next;
    ret_block->used = 1;

    remove_block_from_free_list_by_order(alloc, ret_block, need_order);

    alloc->free[need_order].next = alloc->free[need_order].next->next;

    return (void*)((uintptr_t)ret_block + (uintptr_t)sizeof(free_block_t));
}

int free_buddy_alloc(buddy_alloc_t *alloc, void *ptr){
    free_block_t* block = (free_block_t*)((uintptr_t)ptr - (uintptr_t)sizeof(free_block_t));

    block->next = alloc->free[block->order].next;
    alloc->free[block->order].next = block;

    if (get_buddy(alloc, block, block->order)->used){
        return 0;
    }

    

    return 0;
}