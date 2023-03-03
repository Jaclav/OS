#ifndef IO_H
#define IO_H

#include "stdlib.h"

struct Key {
	Byte character, scancode;
};
typedef struct Key Key;

void putc(Byte character) {
	//TODO: add cursorColor and setCursorColor(Color color)
	if(character == '\n')
		putc('\r');

	asm("int 0x20"
	    :
	    : "a" (0x0171));
}

void puts(const int string) {
	asm("int 0x20"
	    :
	    : "a" ((1<<8)),
	    "b" (string));
}


void puti(int a) {
	if(a == 0) {
		putc('0');
		return;
	}
	if(a < 0) {
		putc('-');
		a *= -1;
	}
	char buffor[20];
	int ptr = 0;
	for(; a != 0; ptr++) {
		buffor[ptr] = (a % 10) + '0';
		a /= 10;
	}
	for(int i = ptr - 1; i >= 0; i--) {
		putc(buffor[i]);
	}
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
	//reset(str);
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