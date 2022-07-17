#ifndef CORE_INTERNAL_H
#define CORE_INTERNAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

#define S_FLOAT(count) (count * sizeof(f32))
#define S_UCHAR(count) (count * sizeof(u8))
#define S_UINT(count) (count * sizeof(u32))

void Panic(char const *fmt, ...);
char* LoadFileText(const char *path);

#endif