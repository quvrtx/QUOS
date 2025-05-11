#ifndef DRIVER_H
#define DRIVER_H

#include <lib/stdint.h>

typedef struct{
    void (*write)(const char* buf, uint32_t count);
    void (*read)(const char* buf, uint32_t count);
} driver;

#endif //DRIVER_H