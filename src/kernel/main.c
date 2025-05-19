#include <os/sh.h>
#include <kernel/drivers/io_port.h>

void kernel(void){
    vga_clear();

    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);

    vga_puts("+------------------------------------------------------------------------------+\n");
    vga_puts("|  QU OS by QuVertex                                                           |\n");
    vga_puts("+------------------------------------------------------------------------------+\n");

    vga_puts("> ");

    while (1){
        asm volatile("hlt");
    }
}