#include <os/sh.h>
#include <global.h>

void kernel(void){
    vga_clear();

    vga_puts("+------------------------------------------------------------------------------+\n");
    vga_puts("|  QU OS by QuVertex                                                           |\n");
    vga_puts("+------------------------------------------------------------------------------+\n");

    vga_puts(" > ");

    init_globals();

    while (1){
        asm volatile("hlt");
    }
}