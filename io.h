#include <stdbool.h>

typedef unsigned char Byte;
typedef unsigned short Word;

#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREEN 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15

struct Position {
    Byte x, y;
};
typedef struct Position Position;

struct Key {
    Byte character, scancode;
};
typedef struct Key Key;

void cls(void);
void setBackgroundColor(Byte color);
void setVideoMode(Byte mode);
Word getMemorySize(void);

Position getCursorPosition(void);
void setCursorPosition(Position position);

void putchar(Byte character);
void print(char *string);
void printInt(int a);

Key getchar(void);

bool strcmp(char *str1, char *str2);
int strlen(char *str);
int stoi(char *str);
void reset(char *str);

int pow(int b, int p);
