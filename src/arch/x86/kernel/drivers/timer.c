#include <kernel/drivers/io_port.h>
#include <global.h>

volatile uint32_t timer_ticks = 0;

void timer_handler_c() {
    timer_ticks++;
    
    if (timer_ticks % 18 == 0) {
        time++;
    }
    
    outb(0x20, 0x20);
}

void timer_install() {
    uint32_t divisor = 1193180 / 18;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}