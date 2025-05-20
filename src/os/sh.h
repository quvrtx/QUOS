#ifndef SH_H
#define SH_H

extern void vga_new_line();
extern void vga_clear();
extern void vga_puts(const char* str);
extern void vga_delc();
extern void vga_putc(char c);
void vga_scroll(int lines);

#endif //SH_H