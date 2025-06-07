#ifndef FS_H
#define FS_H

#include <lib/stddef.h>
#include <lib/stdint.h>

#define MAX_OPENED 256
#define STDIN      0
#define STDOUT     1
#define STDERR     2

enum FDTYPES{
    FD_NONE, FD_FILE, FD_DEVICE, FD_PIPE 
};

static uint16_t current_pos = 3;

typedef struct {
    int type;
    size_t size;
    int flags;
    size_t offset;
    void* buf;
    void* data;
} file_descriptor;

file_descriptor gfdt[MAX_OPENED+3] = {
    {
        .type = FD_FILE,
    },
    {
        .type = FD_FILE
    },
    {
        .type = FD_FILE
    }
};

#endif //FS_H