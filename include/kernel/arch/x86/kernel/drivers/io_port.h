#ifndef PORT_IO_H
#define PORT_IO_H

#include <lib/stdint.h>

u8 inb(u16 port);
u16 inw(u16 port);
u32 inl(u16 port);

void outb(u16 port, u8 value);
void outw(u16 port, u16 value);
void outl(u16 port, u32 value);

void io_wait(void);

#endif
