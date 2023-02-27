#include "io.h"

void cls(void) {
    Position position = {0, 0};
    setCursorPosition(position);
    for(int i = 0; i < 0xffff; i++)
        putc(' ');
    setCursorPosition(position);
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

void putc(Byte character) {
    if(character == '\n')
        putc('\r');

    asm("int 0x10"
        :
        : "a" ((0x0e<<8) | character),
        "b" (10),                       //TODO: do it better
        "c" (0x01));
}

void puts(const int string) {
    char* ptr = (int*)(string);
    while(*ptr != 0) {
        putc(*ptr);
        ptr++;
    }
}

void puti(int a) {
    if(a == 0) {
        putc('0');
        return;
    }
    if(a < 0) {
        putc('-');
        a *= -1;
    }
    char buffor[20];
    int ptr = 0;
    for(; a != 0; ptr++) {
        buffor[ptr] = (a % 10) + '0';
        a /= 10;
    }
    for(int i = ptr - 1; i >= 0; i--) {
        putc(buffor[i]);
    }
}

Key getc(void) {
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

int gets(char *str) {
    //TODO: what when arrows are pressed?
    Key key;
    int ptr = 0;
    reset(str);
    for(;;) {
        key = getc();
        if(key.character == 13) {
            str[ptr] = 0;//end string with null
            putc('\n');
            break;
        }
        if(key.character == 8 && ptr >= 0) { //backspace
            if(ptr > 0) {
                str[ptr] = 0;
                ptr--;
                putc(8);
                putc(' ');
                putc(8);
            }
            continue;
        }
        putc(key.character);
        str[ptr++] = key.character;
    }
    return ptr;
}