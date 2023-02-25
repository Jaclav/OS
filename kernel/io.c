#include "io.h"

void cls(void) {
    Position position = {0, 0};
    setCursorPosition(position);
    for(int i = 0; i < 0xffff; i++)
        printChar(' ');
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

void printChar(Byte character) {
    if(character == '\n')
        printChar('\r');

    asm("int 0x10"
        :
        : "a" ((0x0e<<8) | character),
        "b" (VGA_COLOR_LIGHT_GREEN),
        "c" (0x01));
}

void print(char *string) {
    while(*string != 0) {
        printChar(*string);
        string++;
    }
}

void printInt(int a) {
    if(a == 0) {
        printChar('0');
        return;
    }
    if(a < 0) {
        printChar('-');
        a *= -1;
    }
    char buffor[20];
    int ptr = 0;
    for(; a != 0; ptr++) {
        buffor[ptr] = (a % 10) + '0';
        a /= 10;
    }
    for(int i = ptr - 1; i >= 0; i--) {
        printChar(buffor[i]);
    }
}

Key getKey(void) {
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

int getStr(char *str) {
	//TODO: what when arrows are pressed?
    Key key;
    int ptr = 0;
	reset(str);
    for(;;) {
        key = getKey();
        if(key.character == 13) {
			printChar('\n');
            break;
        }
        if(key.character == 8 && ptr >= 0) { //backspace
            if(ptr > 0) {
                str[ptr] = 0;
                ptr--;
                printChar(8);
                printChar(' ');
                printChar(8);
            }
            continue;
        }
		printChar(key.character);
        str[ptr++] = key.character;
    }
	return ptr;
}