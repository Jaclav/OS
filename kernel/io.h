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

void printChar(Byte character);
void print(char *string);
void printInt(int a);

Key getKey(void);
int getStr(char *str);