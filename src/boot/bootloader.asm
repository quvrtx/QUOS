[bits 32]

section .multiboot
align 4
    dd 0x1BADB002
    dd 0x00000003
    dd -(0x1BADB002 + 0x03)

section .text
global _start

extern kernel_main
extern gdt_descriptor
extern setup_idt
extern setup_pic

_start:
    cli

    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:.protected_mode

.protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, stack_top

    call setup_idt
    call setup_pic
    sti

    call kernel_main

    cli
    hlt

section .bss
align 16
stack_bottom:
    resb 16384
stack_top: