#ifndef IO_H
#define IO_H

#include "types.h"

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

int gets(char *str) {
	//TODO: what when arrows are pressed?
	Key key;
	int ptr = 0;
	for(;;) {
		key = getc();
		if(key.character == 13) {
			str[ptr] = 0;//end string with null
			putc('\n');
			break;
		}
		if(key.character == 8 && ptr >= 0) { //backspace
			if(ptr > 0) {
				str[ptr] = 0;
				ptr--;
				putc(8);
				putc(' ');
				putc(8);
			}
			continue;
		}
		putc(key.character);
		str[ptr++] = key.character;
	}
	return ptr;
}

#endif