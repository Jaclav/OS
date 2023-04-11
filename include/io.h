#ifndef IO_H
#define IO_H

#include "types.h"
#include <stdarg.h>

struct Key {
	Byte character, scancode;
};
typedef struct Key Key;

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
		putc('-');
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
			case 'd': {
				puti(va_arg(va, int));
				break;
			}
			case 'u':
				asm("int 0x20"::"a"(0x200), "b"(va_arg(va, unsigned int)));
				break;
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
	asm("mov ah, 0x00\n"
	    "int 0x16"
	    : "=a" (key)
	    :"a" (0x10));
	return key;
}

//https://cplusplus.com/reference/cstdio/FILE/
#define EOF -1

typedef struct {
	Byte id;
	Byte size;
	Word fpi;
} FILE;

/**
* @brief
*
* @param name file name
* @param mode
* @return FILE*
*/
FILE *fopen(int name, int mode) {
	//https://pubs.opengroup.org/onlinepubs/9699919799/functions/fopen.html
	static FILE file;
	file.fpi = 0;
	asm ("int 0x21\n"
	     "mov bl, ah\n"
	     "xor ah, ah":"=a"(file.id), "=b"(file.size):"a"(0x100), "b"(name));
	if(file.size == 0)
		return NULL;
	return &file;
}

/**
* @brief Read sectors to memory
*
* @param ptr pointer to destination memory
* @param size by now 512B - 1 sector
* @param count number of sectors to read
* @param stream FILE
* @return size_t readed count
*/
size_t fread ( void * ptr, size_t size, size_t count, FILE * stream ) {
	//TODO make operation size Byte not sector
	asm("int 0x21"::"a"(0x0200), "b"(stream->id), "c"(ptr), "d"(count));
}

/**
* @brief Save str (< 512B) to sector FILE->beginSector
*
* @param str string to be saved
* @param stream file to save
* @return int 0 if succesfull
*/
int fputs ( const int str, FILE * stream ) {
	//max size is one sector
	asm("int 0x21"::"a"(0x0300), "b"(stream->id), "c"(str));
}

/**
 * @brief Create file
 *
 * @param str file name
 * @param size file size (in sectors) cannot be 0
 * @return int 0 if succsesfull 1 if not
 */
int create(const int str, size_t size) {
	asm("int 0x21"::"a"(0x0400), "b"(str), "c"(size));
}

#pragma GCC diagnostic pop
#endif