//#include <lib/string.h>
#include <lib/stddef.h>
#include <os/sh.h>

extern void* memcpy(void* dest, const void* src, size_t n);
extern void* memset(void* s, int c, size_t n);
extern size_t strlen(const char* s);
extern int strcmp(const char* s1, const char* s2);
extern char* strcpy(char* dest, const char* src);

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