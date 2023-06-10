/**
 * @file conio.h
 * @brief Console input output control
 * @details Something like https://www.programmingsimplified.com/c/conio.h
 */

#ifndef CONIO_H
#define CONIO_H

#include "types.h"

enum Modes {TextMode = 2, TextColorMode = 16, ColorMode = 19};

typedef struct Cursor {
	Byte x, y;
} Cursor;

typedef union Attributes {
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

/**
 * @brief Get position of Cursor
 * @details DH = row DL = column
 *
 * @return Cursor
 */
Cursor getCursorPosition(void) {
	Cursor cursor;
	asm("int 0x10"
	    : "=d"(cursor)
	    : "a"(0x0300), "b"(0x0));
	return cursor;
}

/**
 * @brief Set position of cursor
 *
 * @param cursor
 */
void setCursorPosition(Cursor cursor) {
	asm("int 0x10"
	    :: "a"(0x0200), "b"(0x0), "d"(cursor));
}

/**
 * @brief Clear screen
 *
 */
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

/**
 * @brief Color put character
 *
 * @param c character
 * @param color
 * @param times how many times put c
 */
void cputc(char c, Color color, Byte times) {
	asm("int 0x10"::"a"(0x0900|c), "b"(0x0000|color), "c"(times));
	//move cursor by 1
	asm("int 0x10"
	    :: "a" (0x0300), "b" (0x0));
	asm("add dl, al\n\
		int 0x10"
	    :: "a"(0x0200|times));
}

/**
 * @brief Color put string
 *
 * @param str
 * @param color
 */
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
