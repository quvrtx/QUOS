[bits 32]

global gdt

gdt_start:
    ; NULL descriptor (обязателен)
    dd 0x00000000
    dd 0x00000000

    ; CS (Kernel Code Segment)
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0b10011010
    db 0b11001111
    db 0x00

    ; DS (Kernel Data Segment)
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0b10010010
    db 0b11001111
    db 0x00

    ; UCS (User Code Segment)
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0b11111010
    db 0b11001111
    db 0x00

    ; UDS (User Data Segment)
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0b11110010
    db 0b11001111
    db 0x00

gdt_end:

gdt:
    dw gdt_end - gdt_start - 1
    dd gdt_start