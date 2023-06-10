#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int size_t;

enum Color {Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, LightGrey, DarkGrey, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, LightBrown, White};
typedef Byte Color;

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef DEBUG
#define DEBUG asm("xchg bx,bx");
#endif

#define __start __attribute__((section("start")))

#endif