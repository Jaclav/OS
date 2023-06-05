#ifndef IO_H
#define IO_H

#include "types.h"
#include <stdarg.h>

union Key {
	struct {
		Byte character: 8;
		Byte scancode: 7;
		Byte available: 1;
	};
};
typedef union Key Key;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"

void putc(Byte character) {
	//TODO: add cursorColor and setCursorColor(Color color)
	asm("mov al, [ebp+8]\n"
	    "int 0x20"::"a"(0));
}

void puts(const int string) {
	asm("mov bx,[ebp+8]\n"
	    "int 0x20"::"a"(0x100));
}

void puti(int a) {
	if(a < 0) {
		asm("mov al, '-'\n"
		    "int 0x20"::"a"(0));
		a *= -1;
	}
	asm("mov bx, [ebp+8]\n"
	    "int 0x20"::"a"(0x200));
}

int printf(const int str, ...) {
//https://cplusplus.com/reference/cstdio/printf/
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
			case 'u':
				asm("int 0x20"::"a"(0x200), "b"(va_arg(va, unsigned int)));
				break;
			case 'b': {
				char num[17];
				int par = va_arg(va, unsigned int);
				for(int i = 0; i < 16; i++) {
					num[15 - i] = par % 2 + '0';
					par /= 2;
				}
				num[16] = 0;
				asm("int 0x20"::"a"(0x100), "b"(num));
				break;
			}
			case 'c':
				asm("xor ah, ah\n"
				    "int 0x20"::"a"(va_arg(va, int)));
				break;
			case 's'://BUG: cannot be string literal why!?
				asm("int 0x20"::"a"(0x100), "b"(va_arg(va, int)));
				break;
			default:
				asm("xor ah,ah\n"
				    "int 0x20"::"a"(*ptr));
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
	Key key;
	asm("int 0x16"
	    :"=a"(key)
	    :"a" (0x0));
	return key;
}

Key getk(void) {
	Key key;
	asm("int 0x16\n"
	    "mov bx, 0\n"
	    "jz .end\n"
	    "mov bx, 1\n"
	    ".end:"
	    :"=a"(key), "=b"(key.available)
	    :"a" (0x0100));
	return key;
}

#pragma GCC diagnostic pop
#endif