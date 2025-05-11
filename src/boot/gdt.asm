section .data

global gdt_descriptor

; GDT 
gdt_start:
    dq 0x0000000000000000

    gdt_code:
        dw 0xFFFF       ; Limit (0-15)
        dw 0x0000       ; Base (0-15)
        db 0x00         ; Base (16-23)
        db 0x9A         ; Access byte (P=1, DPL=00, S=1, Type=1010)
        db 0xCF         ; Flags (G=1, D/B=1, L=0, AVL=0) + Limit (16-19)
        db 0x00         ; Base (24-31)

    gdt_data:
        dw 0xFFFF
        dw 0x0000
        db 0x00
        db 0x92
        db 0xCF
        db 0x00

    gdt_code64:
        dw 0xFFFF
        dw 0x0000
        db 0x00
        db 0x9A
        db 0xAF
        db 0x00

    gdt_data64:
        dw 0xFFFF
        dw 0x0000
        db 0x00
        db 0x92
        db 0xCF
        db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
CODE64_SEG equ gdt_code64 - gdt_start
DATA64_SEG equ gdt_data64 - gdt_start