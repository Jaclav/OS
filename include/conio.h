/**
 * @file conio.h
 * @brief https://www.programmingsimplified.com/c/conio.h
 * @date 2023-04-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CONIO_H
#define CONIO_H

#include "types.h"

#define MODE_TEXT 2
#define MODE_TEXT_COLOR 16
#define MODE_COLOR 19

struct Cursor {
	Byte x, y;
};
typedef struct Cursor Cursor;

typedef union {
	struct {
		Byte foreground: 4;
		Byte background: 3;
		Byte blink: 1;
	};
	Byte attributes;
} Attributes;

void setVideoMode(Byte mode) {
	asm("int 0x10"
	    :
	    :"a"(0x0000|mode));
}

void setColorPalette(Byte color) {
	asm("int 0x10"
	    ::"a"(0x0b00), "b"(color));
}

Cursor getCursorPosition(void) {
	/*
	DH = row
	DL = column
	*/
	Cursor cursor;
	asm("int 0x10"
	    : "=d"(cursor)
	    : "a"(0x0300), "b"(0x0));
	return cursor;
}

void setCursorPosition(Cursor cursor) {
	asm("int 0x10"
	    :: "a"(0x0200), "b"(0x0), "d"(cursor));
}

void cls(void) {
	asm("int 0x10\n\
        cls_l%=:\n\
        mov ax,0\n\
        int 0x20\n\
        loop cls_l%=\n\
        mov ax,0x0200\n\
        int 0x10"
	    :
	    : "a" (0x0200),
	    "b" (0x0),
	    "c" (0xffff),
	    "d" (0x0));
}

void cputc(char c, Color color, Byte times) {
	asm("int 0x10"::"a"(0x0900|c), "b"(0x0000|color), "c"(times));
	//move cursor by 1
	asm("int 0x10"
	    :: "a" (0x0300), "b" (0x0));
	asm("add dl, al\n\
		int 0x10"
	    :: "a"(0x0200|times));
}

void cputs(int str, Color color) {
	char* ptr = (char *)str;
	while(*ptr != 0) {
		asm("int 0x10"::"a"(0x0900|*ptr), "b"(0x0000|color), "c"(1));
		//move cursor by 1
		asm("int 0x10\n\
			mov ah,02\n\
			inc dl\n\
			int 0x10"
		    :: "a" (0x0300), "b" (0x0));
		ptr++;
	}
}

#endif //CONIO_H
