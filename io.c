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
        : "a" (0x02 << 8),
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
    Position position = getCursorPosition();
    position.x++;

    if(character == '\n') {
        position.y++;
        position.x = 0;
        setCursorPosition(position);
        return;
    }

    asm("int 0x10"
        :
        : "a" ((0x0e<<8) | character),
        "b" (0x00),
        "c" (0x01));

    setCursorPosition(position);
}

void puts(const Byte *string) {
    while(*string != 0) {
        putchar(*string);
        string++;
    }
}
