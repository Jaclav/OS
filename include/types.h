#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int size_t;

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef DEBUG
#define DEBUG asm("xchg bx,bx");
#endif

#endif