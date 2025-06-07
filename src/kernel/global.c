#include <mm/buddy.h>
#include <os/sh.h>

int time = 0;

int input_pos = 0;
char* input = (char*)0xFF11;

void init_globals(){
    vga_fputs("%x\n >", input);
}