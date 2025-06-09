#include <lib/stddef.h>
#include <os/sh.h>
#include <lib/string.h>
#include <kernel/drivers/io_port.h>

void execute(char* input){
    if (!strcmp(input, "help")){
        vga_puts("+-------------------------------------+\n");
        vga_puts("|  help - list of commands            |\n");
        vga_puts("|  off - off cpu                      |\n");
        vga_puts("+-------------------------------------+\n");
        return;
    } else if (!strcmp(input, "off")){
        outw(0x604, 0x2000);
    } else {
        vga_colorful_puts("Unknown command\n", 0x04);
    }
}