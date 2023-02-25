#include "stdlib.h"

struct Position {
    Byte x, y;
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