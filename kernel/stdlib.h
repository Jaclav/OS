#ifndef STDLIB_H
#define STDLIB_H

#include <stdbool.h>
#include <stdint.h>

#define NULL 0

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int size_t;

struct Position {
	Word x, y;
};
typedef struct Position Position;


Word getMemorySize(void) {
	// size in kB
	Word size;
	asm("int 0x12"
	    :"=a"(size));
	return size;
}

void udelay(unsigned long usecs) {
	//http://vitaly_filatov.tripod.com/ng/asm/asm_026.13.html
	asm("int 0x15"
	    :
	    :"a"(0x8600), "c"(usecs>>16), "d"((Word)usecs));
}

void cls(void) {
	asm("int 0x10\n\
        cls_l:\n\
        mov ax,0\n\
        int 0x20\n\
        loop cls_l\n\
        mov ax,0x0200\n\
        int 0x10"
	    :
	    : "a" (0x0200),
	    "b" (0x0),
	    "c" (0xffff),
	    "d" (0x0));
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