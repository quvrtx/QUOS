[bits 32]
global kernel_main
extern kernel

kernel_main:
    call kernel
    ret