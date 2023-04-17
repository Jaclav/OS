//TODO: add .bmp pl.wikipedia.org/wiki/Windows_Bitmap
//TODO add get window height aget window length
//https://www.programmingsimplified.com/c/graphics.h
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"

struct Position {
	Word x, y;
};
typedef struct Position Position;

const size_t SIZE_X = 640;
const size_t SIZE_Y = 350;

void writePixel(Position pos, Color color) {
	asm("int 0x10"
	    ::"a"(0x0c00|color), "b"(0), "c"(pos.x), "d"(pos.y));
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

#endif