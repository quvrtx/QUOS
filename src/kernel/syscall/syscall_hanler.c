#include <kernel/syscall/syscall.h>

void handle_syscalls(void) {
    unsigned int index;

    asm volatile (
        "movl %%eax, %[out_index]"
        : [out_index] "=r" (index)
        : 
        : "memory"
    );

    if (index < MAX_SYSCALLS) {
        return;
    }
}