//TODO https://cplusplus.com/reference/cstdio/FILE/
//TODO https://cplusplus.com/reference/cstdio/fread/
#ifndef FS_H
#define FS_H

#include "types.h"
#include "string.h"
#define FILESIZE_MAX 4048
#define EOF -1

typedef struct {
	Word beginSector;
	Word size;
	Word iterator;
	Byte content[FILESIZE_MAX];//do it dynamically
} FILE;

/**
 * @brief
 *
 * @param name
 * @param mode
 * @return FILE*
 */
FILE *fopen(int name, int mode) {
	static FILE file;
	file.beginSector = 0;
	file.size = 0;
	file.iterator = 0;

	return NULL;
}

/**
 * @brief
 *
 * @param stream
 * @return int
 */
int fclose(FILE *file) {
	memset(file->content, 0x90, FILESIZE_MAX);
	return 0;
}

/**
 * @brief
 *
 * @param ptr
 * @param size
 * @param count
 * @param stream
 * @return size_t
 */
size_t fread (void *ptr, size_t size, size_t count, FILE * stream) {

}

/**
 * @brief
 *
 * @param stream
 * @return int
 */
int fgetc (FILE *stream) {

}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
/**
 * @brief read sectors and store them in memory
 *
 * @param ptr pointer to memory where sectors will be stored
 * @param bsector number of first sector (counted from 1)
 * @param count how many sectors load
 * @return size_t number of loaded sectors or 0 when reading error
 */
size_t readSector (void * ptr, size_t bsector, size_t count) {
	/*
	 * ES:BX address to store
	 * AH 2 BIOS read
	 * AL number of sectors to read
	 * CH cylinder
	 * CL number of first sector to store
	 * DH head
	 * DL drive
	 */
	asm("mov bx, ds\n\
		mov	es,	bx\n\
		mov	bx,	[ebp+8]\n\
		mov	ch,	0x0\n\
		mov	ah,	2\n\
		int	0x13\n\
		jnc exit%=\n\
		mov	ax,0\n\
		exit%=:\n\
		xor ah,ah"
	    :
	    :"a"(count), "d"(0), "c"(bsector));
}

#pragma GCC diagnostic pop
#endif