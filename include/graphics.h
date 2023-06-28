/**
 * @file graphics.h
 * @brief Draw pixels in graphics mode
 * @details Something like https://www.programmingsimplified.com/c/graphics.h
 * @todo add .bmp pl.wikipedia.org/wiki/Windows_Bitmap
 * @todo add get window height and window length
 */
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"

/**
 * @brief Position on screen
 */
typedef struct Position {
	Word x;
	Word y;
} Position;

const size_t SIZE_X = 640;
const size_t SIZE_Y = 350;

/**
 * @brief Draw single pixel
 *
 * @param pos
 * @param color
 */
void writePixel(Position pos, Color color) {
	asm("int 0x10"
	    ::"a"(0x0c00|color), "b"(0), "c"(pos.x), "d"(pos.y));
}

/**
 * @brief Draw picture from raw pixel table
 *
 * @param begin left right corner to begin drawing
 * @param data array of pixels
 * @param width
 * @param height
 */
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