//TODO: add .bmp pl.wikipedia.org/wiki/Windows_Bitmap
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "io.h"
#include "types.h"

typedef Byte Color;

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
	Word x, y;
};
typedef struct Position Position;

const size_t SIZE_X = 640;
const size_t SIZE_Y = 350;

void setVideoMode(Byte mode) {
    asm("int 0x10"
        :
        :"a"(mode));
}

void setColorPalette(Byte color) {
    asm("int 0x10"
        :
        :"a"(0x0b << 8),
        "b"(color));
}

void writePixel(Position pos, Color color) {
    asm("int 0x10"
        :
        :"a"(0xc<<8|color), "b"(0), "c"(pos.x), "d"(pos.y));
}

void draw(Position begin, Color *data, size_t width, size_t height) {
    //begin is position of first up right corner pixel
    Position pos;
    for(size_t y = 0; y < height; y++) {
        for(size_t x = 0; x < width; x++) {
            pos.x = x + begin.x;
            pos.y = y + begin.y;
            writePixel(pos, data[y * (width) + x]);
        }
    }
}

Position getCursorPosition(void) {
	Position position;
	Word dx;

	asm("int 0x10"
	    : "=d"(dx)
	    : "a" (0x03 << 8),
	    "b" (0x0));
	position.y = dx >> 8;
	position.x = (Byte)dx;

	return position;
}

void setCursorPosition(Position position) {
	asm("int 0x10"
	    :
	    : "a" (0x0200),
	    "b" (0x0),
	    "d" ((position.y << 8) | position.x));
}

#endif