#ifndef STDLIB_H
#define STDLIB_H

#include <stdbool.h>
#include <stdint.h>

#define NULL 0

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int size_t;

struct Position {
    Word x, y;
};
typedef struct Position Position;

Word getMemorySize(void);

void udelay(unsigned long usecs);

void cls(void);

Position getCursorPosition(void);

void setCursorPosition(Position position);

#endif