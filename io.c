#include "io.h"

void cls(void) {
    Position position = {0, 0};
    setCursorPosition(position);
    asm("int 0x10"
        :
        : "a" ((0x09 << 8) | ' '),
        "b" (0x0),
        "c" (0x640));
}

void setColor(Byte color) {
    asm("int 0x10"
        :
        :"a"(0x0b << 8),
        "b"(color | 0x0000));
}

void setVideoMode(Byte mode) {
    asm("int 0x10"
        :
        :"a"(mode | 0x0000));
}

Position getCursorPosition(void) {
    Position position;
    Word dx;

    asm("int 0x10"
        : "=d"(dx)
        : "a" (0x02 << 8), //I don't know why it work
        "b" (0x0));
    position.y = dx >> 8;
    position.x = (Byte)dx;

    return position;
}

void setCursorPosition(Position position) {
    asm("int 0x10"
        :
        : "a" (0x02 << 8),
        "b" (0x0),
        "d" ((position.y << 8) | position.x));
}

void putchar(Byte character) {
    if(character == '\n')
        putchar('\r');

    asm("int 0x10"
        :
        : "a" ((0x0e<<8) | character),
        "b" (0x00),
        "c" (0x01));
}

void print(const char *string) {
    while(*string != 0) {
        putchar(*string);
        string++;
    }
}

void printInt(int a) {
    if(a == 0) {
        putchar('0');
        return;
    }
    if(a < 0) {
        putchar('-');
        a *= -1;
    }
    char buffor[20] = {};
    int ptr = 0;
    for(; a != 0; ptr++) {
        buffor[ptr] = (a % 10) + '0';
        a /= 10;
    }
    for(int i = ptr - 1; i >= 0; i--) {
        putchar(buffor[i]);
    }
}

Key getchar(void) {
    Word ax;
    asm("mov ah, 0x00\n\
    int 0x16"
        : "=a" (ax)
        :"a" (0x00));
    Key key;
    key.character = (Byte) ax;
    key.scancode = ax >> 8;
    return key;
}