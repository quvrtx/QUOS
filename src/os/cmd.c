#include <lib/string.h>
#include <lib/stddef.h>
#include <os/sh.h>

void execute(char input[512]){
    char cmd[128];

    int i;
    cmd[i+1] = '\0';

    if (strcmp(cmd, "help")){
        vga_puts("+-------------------------------------+\n");
        vga_puts("|  help - list of commands            |\n");
        vga_puts("+-------------------------------------+\n");
        return;
    }
}