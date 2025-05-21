#include <syscall/syscall.h>

int write(int fd, void* buf, uint32_t count){
    return 0;
}

int read(int fd, const void* buf, uint32_t count){
    return 0;
}

syscall_handler_t handlers[] = {
    
};