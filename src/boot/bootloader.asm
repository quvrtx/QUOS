[bits 32]

section .multiboot
align 4
    multiboot_header:
        dd 0x1BADB002             ; Magic
        dd 0x00000003             ; Flags
        dd -(0x1BADB002 + 0x03)   ; Checksum

section .text
global _start

extern kernel_main
extern gdt_descriptor
extern int80_handler

_start:
    cli
    lgdt [gdt_descriptor]
    lidt [int80_handler]
    mov esp, stack_top
    call kernel_main
    cli
    hlt

section .bss
align 16
stack_bottom:
    resb 16384
stack_top: