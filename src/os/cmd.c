#include <lib/stddef.h>
#include <os/sh.h>
#include <lib/string.h>

int check_cmd(char* input, char* cmd){
    int res = 0;

    for (int i = 0; input[i] != '\0' && cmd[i] != '\0'; i++){
        if (input[i] == cmd[i]){
            res = 1;
        }
        else{
            res = 0;
        }
    }
    return res;
}

void execute(char input[512]){
    char cmd[128];

    int i;
    cmd[i+1] = '\0';

    if (check_cmd(cmd, "help")){
        vga_puts("+-------------------------------------+\n");
        vga_puts("|  help - list of commands            |\n");
        vga_puts("+-------------------------------------+\n");
        return;
    }
}