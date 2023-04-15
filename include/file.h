#ifndef FILE_H
#define FILE_H

#include "types.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"

//https://cplusplus.com/reference/cstdio/FILE/
#define EOF -1

typedef struct {
	Byte id;
	Byte size;
	Word fpi;
} FILE;

FILE *open(int name, int mode) {
	//https://pubs.opengroup.org/onlinepubs/9699919799/functions/fopen.html
	static FILE file;
	file.fpi = 0;
	asm ("int 0x21\n"
	     "mov bl, ah\n"
	     "xor ah, ah":"=a"(file.id), "=b"(file.size):"a"(0x100), "b"(name));
	if((char)file.size < 0)
		return NULL;
	return &file;
}

int read ( void * ptr, size_t count, FILE * stream ) {
	asm("int 0x21"::"a"(0x0200), "b"(stream->id), "c"(ptr), "d"(count));
}

int write ( int ptr, size_t count, FILE * stream ) {
	asm("int 0x21"::"a"(0x0300), "b"(stream->id), "c"(ptr), "d"(count));
}

/**
 * @brief Create file
 *
 * @param str file name
 * @param size file size >= 0
 * @return int 0 if succsesfull 1 if not
 */
int create(const int str, size_t size) {
	asm("int 0x21"::"a"(0x0400), "b"(str), "c"(size));
}

#pragma GCC diagnostic pop
#endif