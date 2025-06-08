#include <kernel/drivers/io_port.h>
#include <lib/stdint.h>
#include <kernel/drivers/timer.h>

extern void init_pic();
extern void empty_handler();

extern void keyboard_handler();

typedef struct __attribute__((packed)) idt_entry{
    u16 low_handler;
    u16 selector;
    u8 zero;
    u8 flags;
    u16 high_handler;
} IDT_ENTRY;

typedef struct __attribute__((packed)) {
    u16 limit;
    u32 base;
} IDT_DESCRIPTOR;

IDT_ENTRY idt[256];
IDT_DESCRIPTOR idtptr;

void idt_set_gate(int num, void (*handler)(), u8 flags, u16 selector){
    idt[num].low_handler = (u32)handler & 0xFFFF,
    idt[num].selector = selector,
    idt[num].zero = 0x00,
    idt[num].flags = flags,
    idt[num].high_handler = ((u32)handler >> 16) & 0xFFFF;
}

void init_idt(){
    init_pic();

    /* EXCEPTIONS (0-31) */
    for (int i = 0; i < 32; i++){
        idt_set_gate(i, empty_handler, 0x8E, 0x08);
    }
    
    /* IRQS (32-47) */
    idt_set_gate(33, keyboard_handler, 0x8E, 0x08);

    /* INT (48-256) */
    for (int i = 48; i < 256; i++){
        idt_set_gate(i, empty_handler, 0x8E, 0x08);
    }

    idtptr.limit = sizeof(idt) - 1;
    idtptr.base = (u32)&idt;

    asm volatile("lidt %0" : : "m" (idtptr));
}
