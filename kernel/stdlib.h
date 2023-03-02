#ifndef STDLIB_H
#define STDLIB_H

#include <stdbool.h>
#include <stdint.h>

#define NULL 0

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int size_t;

Word getMemorySize(void);

void udelay(unsigned long usecs);

#endif