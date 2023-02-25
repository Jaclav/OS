#include "graphics.h"
const size_t SIZE_X = 640;
const size_t SIZE_Y = 350;

void setVideoMode(Byte mode) {
    asm("int 0x10"
        :
        :"a"(mode | 0x0000));
}

void setColorPalette(Byte color) {
    asm("int 0x10"
        :
        :"a"(0x0b << 8),
        "b"(color | 0x0000));
}

void writePixel(Position pos, Color color) {
    asm("int 0x10"
        :
        :"a"(0xc<<8|color), "b"(0), "c"(pos.x), "d"(pos.y));
}

void draw(Position begin, Color *data, size_t sizeX, size_t sizeY) {
    Position pos = {};
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {
            pos.x = begin.x + x;
            pos.y = begin.y + y;
            writePixel(pos, data[x * sizeX + y]);
        }
    }
}