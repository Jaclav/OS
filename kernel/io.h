#ifndef IO_H
#define IO_H

#include "stdlib.h"

struct Position {
    Word x, y;
};
typedef struct Position Position;

struct Key {
    Byte character, scancode;
};
typedef struct Key Key;

Position getCursorPosition(void);
void setCursorPosition(Position position);

void putc(Byte character);
void puts(const char *string);
void puti(int a);

Key getc(void);
int gets(char *str);

#endif