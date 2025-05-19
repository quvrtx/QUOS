#ifndef _STRING_H
#define _STRING_H

#include <lib/stddef.h>

extern void* memcpy(void* dest, const void* src, size_t n);
extern void* memset(void* s, int c, size_t n);
extern size_t strlen(const char* s);
extern int strcmp(const char* s1, const char* s2);
extern char* strcpy(char* dest, const char* src);

#endif // _STRING_H