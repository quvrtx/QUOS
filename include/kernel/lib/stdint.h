#ifndef STDINT_H
#define STDINT_H

typedef signed char i8;
typedef unsigned char u8;

typedef short i16;
typedef unsigned short u16;

typedef int i32;
typedef unsigned int u32;

#if defined(__x86_64__) || defined(__aarch64__) || defined(__LP64__)
typedef long i64;
typedef unsigned long u64;
#else
typedef long long i64;
typedef unsigned long long u64;
#endif

#if __SIZEOF_POINTER__ == 4
typedef u32 uintptr_t;
typedef i32 intptr_t;
#elif __SIZEOF_POINTER__ == 8
typedef uint64_t uintptr_t;
typedef int64_t intptr_t;
#else
#error "Unsupported pointer size"
#endif

#if __SIZEOF_POINTER__ == 8
typedef uint64_t uintmax_t;
typedef int64_t intmax_t;
#else
typedef u32 uintmax_t;
typedef i32 intmax_t;
#endif

#define INT8_MIN   (-128)
#define INT16_MIN  (-32768)
#define INT32_MIN  (-2147483647 - 1)
#define INT64_MIN  (-9223372036854775807LL - 1)

#define INT8_MAX   127
#define INT16_MAX  32767
#define INT32_MAX  2147483647
#define INT64_MAX  9223372036854775807LL

#define UINT8_MAX  255
#define UINT16_MAX 65535
#define UINT32_MAX 4294967295U
#define UINT64_MAX 18446744073709551615ULL

#endif //STDINT_H