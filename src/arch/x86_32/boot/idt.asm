section .text
bits 32

extern timer_handler_c
extern keyboard_handler_c
extern syscall_handler_c

global setup_idt
global default_handler
global timer_handler
global keyboard_handler
global setup_pic

default_handler:
    pusha
    
    mov eax, [esp+32]
    
    cmp eax, 32
    jb .end
    
    mov al, 0x20
    out 0x20, al
    
    cmp eax, 40
    jb .end
    out 0xA0, al
    
.end:
    popa
    iret

syscall_handler:
    pusha
    push ds
    push es
    push fs
    push gs

    push edx
    push ecx
    push ebx
    push eax
    
    call syscall_handler_c
    
    add esp, 16
    
    pop gs
    pop fs
    pop es
    pop ds
    popa
    iret

timer_handler:
    pusha
    push ds
    push es
    push fs
    push gs
    
    call timer_handler_c
    
    mov al, 0x20
    out 0x20, al
    
    pop gs
    pop fs
    pop es
    pop ds
    popa
    iret

keyboard_handler:
    pusha
    push ds
    push es
    push fs
    push gs
    
    in al, 0x60
    movzx eax, al
    
    push eax
    call keyboard_handler_c
    add esp, 4
    
    mov al, 0x20
    out 0x20, al
    
    pop gs
    pop fs
    pop es
    pop ds
    popa
    iret

setup_idt:
    mov ecx, 256
    mov edi, idt
    mov eax, default_handler
    
.set_entry:
    mov word [edi], ax
    mov word [edi+2], 0x08
    mov byte [edi+4], 0
    mov byte [edi+5], 0x8E
    shr eax, 16
    mov word [edi+6], ax
    shr eax, 16
    add edi, 8
    loop .set_entry

    mov eax, syscall_handler
    mov word [idt + 0x80*8], ax
    mov word [idt + 0x80*8 + 2], 0x08
    mov byte [idt + 0x80*8 + 4], 0
    mov byte [idt + 0x80*8 + 5], 0xEE
    shr eax, 16
    mov word [idt + 0x80*8 + 6], ax

    mov eax, timer_handler
    mov word [idt + 0x20*8], ax
    mov word [idt + 0x20*8 + 2], 0x08
    mov byte [idt + 0x20*8 + 4], 0
    mov byte [idt + 0x20*8 + 5], 0x8E
    shr eax, 16
    mov word [idt + 0x20*8 + 6], ax

    mov eax, keyboard_handler
    mov word [idt + 0x21*8], ax
    mov word [idt + 0x21*8 + 2], 0x08
    mov byte [idt + 0x21*8 + 4], 0
    mov byte [idt + 0x21*8 + 5], 0x8E
    shr eax, 16
    mov word [idt + 0x21*8 + 6], ax

    lidt [idt_descriptor]
    ret

setup_pic:
    mov al, 0x11
    out 0x20, al
    out 0xA0, al
    
    mov al, 0x20 
    out 0x21, al
    mov al, 0x28
    out 0xA1, al
    
    mov al, 0x04
    out 0x21, al
    mov al, 0x02
    out 0xA1, al
    
    mov al, 0x01
    out 0x21, al
    out 0xA1, al
    
    mov al, 0xFC
    out 0x21, al
    mov al, 0xFF
    out 0xA1, al
    
    ret

section .data
idt_descriptor:
    dw 256 * 8 - 1
    dd idt


idt:
    times 256 dq 0