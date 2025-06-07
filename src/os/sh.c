#include <os/sh.h>
#include <lib/stdarg.h>
#include <kernel/drivers/io_port.h>

typedef volatile char vchar;

#define WIDTH 80
#define HEIGHT 25

#define CURRENT_POS (cursor.x + cursor.y* WIDTH)

vchar* const vga = (vchar*)0xB8000;

struct {
    int x, y;
} cursor = {
    .x = 0,
    .y = 0
};

void vga_fputs(const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 'd': {
                    int num = va_arg(args, int);
                    char buffer[32];
                    int j = 0;
                    
                    if (num < 0) {
                        vga_putc('-');
                        num = -num;
                    }
                    
                    do {
                        buffer[j++] = '0' + (num % 10);
                        num /= 10;
                    } while (num > 0);
                    
                    while (j > 0) {
                        vga_putc(buffer[--j]);
                    }
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    vga_puts(str);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    vga_putc(c);
                    break;
                }
                case 'x': {
                    unsigned int num = va_arg(args, unsigned int);
                    char hex_digits[] = "0123456789ABCDEF";
                    
                    vga_puts("0x");
                    for (int shift = 28; shift >= 0; shift -= 4) {
                        char c = hex_digits[(num >> shift) & 0xF];
                        vga_putc(c);
                    }
                    break;
                }
                case '%': {
                    vga_putc('%');
                    break;
                }
                default: {
                    vga_putc('%');
                    vga_putc(format[i]);
                    break;
                }
            }
        } else {
            vga_putc(format[i]);
        }
    }

    va_end(args);
}

void vga_move_cursor(int x, int y) {
    uint16_t pos = y * WIDTH + x;
    
    if (x >= WIDTH) x = WIDTH - 1;
    if (y >= HEIGHT) y = HEIGHT - 1;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    
    pos = y * WIDTH + x;
    
    outb(0x3D4, 0x0F);
    io_wait();
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    
    outb(0x3D4, 0x0E);
    io_wait();
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}
void vga_new_line() {
    cursor.x = 0;
    cursor.y += 1;
    
    if (cursor.y >= HEIGHT) {
        vga_scroll(1);
        cursor.y = HEIGHT - 1;
    }
}

void vga_clear() {
    for (int i = 0; i < WIDTH * HEIGHT * 2; i += 2) {
        vga[i] = ' ';
        vga[i+1] = 0x07;
    }
    cursor.x = 0;
    cursor.y = 0;
}

void vga_delc(){
    vga[CURRENT_POS*2-2] = ' ';
    vga[CURRENT_POS*2-1] = 0x07;
    cursor.x--;
    vga_move_cursor(cursor.x, cursor.y);
}

void vga_putc(char c) {
    switch (c) {
        case '\n':
            vga_new_line();
            return;
        case '\r':
            cursor.x = 0;
            return;
    }

    if (cursor.x >= WIDTH) {
        vga_new_line();
    }

    if (cursor.y >= HEIGHT) {
        vga_scroll(1);
        cursor.y = HEIGHT - 1;
    }

    vga[CURRENT_POS * 2] = c;
    vga[CURRENT_POS*2+1] = 0x07;

    cursor.x++;

    vga_move_cursor(cursor.x, cursor.y);
}

void vga_puts(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        vga_putc(str[i]);
    }
}

void vga_scroll(int lines) {
    if (lines >= HEIGHT) {
        lines = HEIGHT - 1;
    }

    for (int y = lines; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int src_pos = (y * WIDTH + x) * 2;
            int dst_pos = ((y - lines) * WIDTH + x) * 2;
            vga[dst_pos] = vga[src_pos];
            vga[dst_pos+1] = vga[src_pos+1];
        }
    }

    for (int y = HEIGHT - lines; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int pos = (y * WIDTH + x) * 2;
            vga[pos] = ' ';
            vga[pos+1] = 0x07;
        }
    }
    
    if (cursor.y >= lines) {
        cursor.y -= lines;
    } else {
        cursor.y = 0;
    }
}