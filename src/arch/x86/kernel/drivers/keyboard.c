#include <kernel/drivers/io_port.h>
#include <os/sh.h>
#include <os/cmd.h>
#include <kernel/mm/mm.h>
#include <lib/stdint.h>

char* input = (char*)0xFF11;
int input_pos = 0;

void clear_input(){
    for (int i = 0; i < 512; i++){
        input[i] = '\0';
    }
    input_pos = 0;
}

void init_keyboard(){
    //input = (void*)kmalloc(512);

    clear_input();

    vga_fputs("%x\n-> ", (uintptr_t)input);
}

char keyboard_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_handler_c() {
    u8 scancode = inb(0x60);
    
    if (scancode < 128) {
        char c = keyboard_map[scancode];
        if (c != 0) {
            switch (c){
                case '\b':
                    if (input_pos != 0){
                        input[input_pos] = '\0';
                        input_pos--;
                        vga_delc();
                    }
                    break;
                
                case '\n':
                    vga_new_line();
                    
                    execute(input);

                    clear_input();
                    vga_fputs("-> ");
                    break;
                default:
                    input[input_pos] = c;
                    input_pos++;
                    input[input_pos] = '\0';
                    vga_putc(c, 0x07);
            }
        }
    }

    outb(0x20, 0x20);
}
