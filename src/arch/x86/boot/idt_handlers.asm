global empty_handler
empty_handler:
    iret

global keyboard_handler
keyboard_handler:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    extern keyboard_handler_c
    call keyboard_handler_c

    pop gs
    pop fs
    pop es
    pop ds
    popa
    iret  