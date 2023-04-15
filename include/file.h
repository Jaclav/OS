#ifndef FILE_H
#define FILE_H

#include "types.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"

//https://cplusplus.com/reference/cstdio/FILE/
//TODO: remove file->size;

typedef struct {
	Byte id;
	Byte size;
	Word fpi;
} FILE;

FILE *open(int filename, int mode) {
	//https://pubs.opengroup.org/onlinepubs/9699919799/functions/fopen.html
	static FILE file;
	file.fpi = 0;
	asm ("int 0x21\n"
	     "mov bl, ah\n"
	     "xor ah, ah":"=a"(file.id), "=b"(file.size):"a"(0x100), "b"(filename));
	if((char)file.size < 0)
		return NULL;
	return &file;
}

int read ( int buf, size_t count, FILE * stream ) {
	asm("int 0x21"::"a"(0x0200), "b"(stream->id), "c"(buf), "d"(count));
}

int write ( int buf, size_t count, FILE * stream ) {
	asm("int 0x21"::"a"(0x0300), "b"(stream->id), "c"(buf), "d"(count));
}

/**
 * @brief Create file
 *
 * @param filename file name
 * @param size file size >= 0
 * @return int 0 if succsesfull
 */
int create(const int filename, size_t size) {
	asm("int 0x21"::"a"(0x0400), "b"(filename), "c"(size));
}

/**
 * @brief Remove file
 *
 * @param filename file name
 * @return int 0 if success
 */
int remove(const int filename) {
	asm("int 0x21"::"a"(0x0500), "b"(filename));
}

#pragma GCC diagnostic pop
#endif