#include <kernel/syscall/syscall.h>

int write(int fd, void* buf, u32 count){
    return 0;
}

int read(int fd, const void* buf, u32 count){
    return 0;
}

syscall_handler_t handlers[] = {
    
};
