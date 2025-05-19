#ifndef SH_H
#define SH_H

void vga_new_line();
void vga_clear();
void vga_puts(const char* str);
void vga_delc();
void vga_putc(char c);
void vga_scroll(int lines);

#endif //SH_H