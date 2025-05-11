#ifndef SYSCALL_H
#define SYSCALL_H

#include <lib/stdint.h>

#define MAX_SYSCALLS 256

typedef union{
    int (*write)
} syscall_handler_t;

extern void handle_syscall();
extern syscall_handler_t handlers[];

#endif //SYSCALL_H