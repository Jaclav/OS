#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "io.h"
#include "stdlib.h"

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

extern const size_t SIZE_X;
extern const size_t SIZE_Y;

void setVideoMode(Byte mode);
void setColorPalette(Byte color);

void writePixel(Position pos, Color color);
void draw(Position begin, Color *data, size_t width, size_t height);

#endif