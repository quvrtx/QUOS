#include <os/sh.h>
#include <kernel/mm/mm.h>
#include <kernel/drivers/keyboard.h>

void kernel(void){
    vga_clear();

    vga_puts("+------------------------------------------------------------------------------+\n");
    vga_puts("|  QU OS by QuVertex                                                           |\n");
    vga_puts("+------------------------------------------------------------------------------+\n");

    vga_puts("-> ");

    init_allocator((void*)0xFF00, 5000);
    init_keyboard();

    while (1){
        asm volatile("hlt");
    }
}