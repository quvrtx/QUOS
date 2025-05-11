

section .text

extern handle_syscall

global int80_handler
int80_handler:
    pusha
    push ds
    push es
    push fs
    push gs

    push edx
    push ecx
    push ebx
    push eax
    
    call handle_syscall

    add esp, 16

    pop gs
    pop fs
    pop es
    pop ds
    popa

    iret

setup_idt:
    mov eax, int80_handler
    mov word [idt + 0x80 * 8], ax
    mov word [idt + 0x80 * 8 + 2], 0x08
    mov byte [idt + 0x80 * 8 + 4], 0
    mov byte [idt + 0x80 * 8 + 5], 0x8E
    shr eax, 16
    mov word [idt + 0x80 * 8 + 6], ax

    ; Загружаем IDT
    lidt [idt_descriptor]
    ret

section .data
idt_descriptor:
    dw 256 * 8 - 1
    dd idt

idt:
    times 256 dq 0