#include "graphics.h"

void writePixel(Position pos, Color color) {
    asm("int 0x10"
        :
        :"a"(0xc<<8|color), "b"(0), "c"(pos.x), "d"(pos.y));
}