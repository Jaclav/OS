#include "io.h"

void cls(void) {
    Position position = {0, 0};
    setCursorPosition(position);
    for(int i = 0; i < 0xffff; i++)
        putchar(' ');
    setCursorPosition(position);
}

void setBackgroundColor(Byte color) {
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

Word getMemorySize(void) {
    // size in kB
    Word size;
    asm("int 0x12"
        :"=a"(size));
    return size;
}

Position getCursorPosition(void) {
    Position position;
    Word dx;

    asm("int 0x10"
        : "=d"(dx)
        : "a" (0x03 << 8), //I don't know why it work
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
        "b" (VGA_COLOR_CYAN),
        "c" (0x01));
}

void print(char *string) {
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
    char buffor[20];
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
        :"a" (0x10));
    Key key;
    key.character = (Byte) ax;
    key.scancode = ax >> 8;
    return key;
}

bool strcmp(char *str1, char *str2) {
    while(*str1 != 0 && *str2 != 0) {
        if(*str1 != *str2)
            return false;
        str1++;
        str2++;
    }
    if(*str1 != *str2)
        return false;
    return true;
}

int strlen(char *str) {
    if(*str == 0)
        return 0;
    int size = 0;
    for(; *str != 0; str++, size++) {}
    return size;
}

int stoi(char *str) {
    int num = 0;
    int size = strlen(str) - 1;
    for(int i = 0; i <= size; i++) {
        num += pow(10, (size - i)) * (str[i] - '0');
    }
    return num;
}

void reset(char *str) {
    while(*str != 0) {
        *str = 0;
        str++;
    }
}

int pow(int b, int p) {
    int ret = 1;
    for(int i = 0; i < p; i++) {
        ret *= b;
    }
    return ret;
}