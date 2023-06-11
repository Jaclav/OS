/**
 * @file file.h
 * @brief File IO
 * @todo remove FILE as pointer
 */
#ifndef FILE_H
#define FILE_H

#include "types.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"

//https://cplusplus.com/reference/cstdio/FILE/

/**
 * @brief Information about file
 */
typedef struct {
	Byte id;/**< file's id*/
	Word size;/**< size in Bytes*/
} FILE;

/**
 * @brief Open file
 *
 * @param filename
 * @param mode
 * @todo add modes, see sys_write() todo
 * @return FILE*
 */
FILE *open(int filename, int mode) {
	//https://pubs.opengroup.org/onlinepubs/9699919799/functions/fopen.html
	static FILE file;
	short val;
	asm ("int 0x21":"=a"(val):"a"(0x100), "b"(filename));
	if(val < 0)
		return NULL;
	file.id = (Byte)val;
	file.size = (val >> 8) * 512;//sys_open returns size in Sectors
	return &file;
}

/**
 * @brief Read from file
 *
 * @param buf pointer to destinated memory
 * @param size to read in bytes
 * @param stream FILE
 * @return int actually readed Bytes or -error
 */
int read ( int buf, size_t size, FILE * stream ) {
	asm("int 0x21"::"a"(0x0200), "b"(stream->id), "c"(buf), "d"(size));
}


/**
 * @brief Write to file
 *
 * @param buf pointer to source memory
 * @param size to write in Bytes, file will be rounded to whole sectors
 * @param stream FILE
 * @return int actually writted Bytes or -error
 */
int write ( int buf, size_t size, FILE * stream ) {
	asm("int 0x21"::"a"(0x0300), "b"(stream->id), "c"(buf), "d"(size));
}

/**
 * @brief Create file
 *
 * @param filename
 * @param size in sectors(512B) >=0
 * @return int new file's id or -error
 */
int create(const int filename, size_t size) {
	asm("int 0x21"::"a"(0x0400), "b"(filename), "c"(size));
}

/**
 * @brief Remove file
 *
 * @param filename
 * @return int 0 if success or -error
 */
int remove(const int filename) {
	asm("int 0x21"::"a"(0x0500), "b"(filename));
}

#pragma GCC diagnostic pop
#endif