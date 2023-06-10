/**
 * @file types.h
 * @brief usefull macros, definitions, enums etc.
 */
#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int size_t;

typedef enum {Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, LightGrey, DarkGrey, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, LightBrown, White} Color;

/**
 * @brief pointer to NULL
 * @todo Make 0th Byte always zero for safety reason
 */
#ifndef NULL
#define NULL ((void *) 0)
#endif

/**
 * @brief debug breakpoint in Bochs
 */
#ifndef DEBUG
#define DEBUG asm("xchg bx,bx");
#endif

/**
 * @brief Must be before main function of program to put this function as first in binary file
 */
#define __start __attribute__((section("start")))

#endif