#ifndef IO_H
#define IO_H

#include "types.h"
#include <stdarg.h>

struct Key {
	Byte character, scancode;
};
typedef struct Key Key;

void putc(Byte character) {
	//TODO: add cursorColor and setCursorColor(Color color)
	asm("mov ah,0\n\
		int 0x20"
	    :: "a" (character));
}

void puts(const int string) {
	asm("int 0x20"
	    ::"a"(0x0100), "b"(string));
}

void puti(int a) {
	asm("int 0x20"
	    ::"a"(0x0200), "b"(a));
}

int printf(const int str, ...) {
//TODO: add more specifiers https://cplusplus.com/reference/cstdio/printf/
	va_list va;
	va_start(va, str);

	char* ptr = str;
	while(*ptr != 0) {
		if(*ptr == '%') {
			switch(*(ptr + 1)) {
			case 'i':
			case 'd':
				puti(va_arg(va, int));
				break;
			case 's'://BUG: cannot be string literal why!?
				puts(va_arg(va, int));
				break;
			default:
				putc(*ptr);
				break;
			}
			ptr++;
		}
		else
			putc(*ptr);
		ptr++;
	}
	va_end(va);
	return 0;
}

//TODO: add separate getchar and getKey
Key getc(void) {
	Word ax;
	asm("mov ah, 0x00\n\
    int 0x16"
	    : "=a" (ax)
	    :"a" (0x10));
	Key key;
	key.character = (Byte) ax;
	key.scancode = ax >> 8;
	return key;
}

#endif