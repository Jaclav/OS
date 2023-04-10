/**
 * @file fs.h
 * @author Jaclav
 * @brief File system header
 * @version 0.1
 * @date 2023-04-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef FS_H
#define FS_H

#ifndef KERNEL
#error Only kernel should use this file!
#endif

#include <types.h>
#include <io.h>
#include <string.h>

#ifndef KERNEL_ADDRESS
#define KERNEL_ADDRESS 0
#endif

#define FILENAME_MAX 16
#define FILES_MAX 37

//do it static - only this file should be able to use this
struct {
	char name[FILENAME_MAX];
	Byte track;
	Byte beginSector;
	Byte size;
} files[FILES_MAX];

bool map[FILES_MAX];

size_t numberOfFiles = 0;
const Byte *FAT = 0x0;//first 512 Bytes is file table

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"

void loadFAT() {
	numberOfFiles = 0;
	if(FAT[0] != 0xcf || FAT[1] != 0xaa || FAT[2] != 0x55)
		puts("ERROR: wrong FAT table format!");
	for(; (FAT[numberOfFiles * 19 + 3] != 0) && numberOfFiles * 19 + 3 < 512; numberOfFiles++) {
		memncpy((char *)(files + numberOfFiles), (char *)(FAT + 3 + numberOfFiles * 19), 19);
		for(int i = 0; i < files[numberOfFiles].size; i++) {
			map[files[numberOfFiles].beginSector + i] = true;
		}
	}
}

static int sys_fopen(const int filename) {
	for(size_t i = 0; i < numberOfFiles; i++) {
		if(strcmp(files[i].name, filename)) {
			return files[i].size << 8 | i;
		}
	}
	return 0;
}

static int sys_read(int id, void *ptr, int count) {
	/*
	* ES:BX address to store
	* AH 2 BIOS read
	* AL number of sectors to read
	* CH cylinder
	* CL number of first sector to store
	* DH head
	* DL drive
	*/
	asm("xor ch, ch\n"
	    "xor dh, dh\n"
	    "mov ah, 2\n"
	    "int 0x13\n"
	    "jnc exit%=\n"
	    "mov ax, 0\n"
	    "exit%=:\n"
	    "	xor ah,ah\n"
	    "	mov [ebp-24], ax"
	    ::"a"(count), "b"(ptr), "c"(files[id].beginSector), "d"(files[id].track));
}

static void sys_write(const Byte id, const int str) {
	Byte toSave[512];
	memset(toSave, 0, 512);
	strncpy((char *)toSave, str, 512);
	/*
	* AH = 03h
	* AL = Number of sectors to write
	* CH = Cylinder number (10 bit value, upper 2 bits in CL)
	* CL = Starting sector number
	* DH = Head number
	* DL = Drive number
	* ES:BX = Address of memory buffer
	*/
	asm("mov es, si\n"
	    "xor ch, ch\n"
	    "xor dh, dh\n"
	    "mov ah, 3\n"
	    "int 0x13"
	    ::"a"(1), "b"(toSave), "c"(files[id].beginSector), "d"(files[id].track), "S"(KERNEL_ADDRESS));
}

static int sys_create(const int str, size_t size) {
	if(size == 0) {
		return 1;
	}
	Byte beginSector = 0;//store first sector of unused cx sectors
	for(int i = 1; i < FILES_MAX; i++) {
		if(strcmp(files[i].name, str)) {
			beginSector = 0;
			break;
		}
		beginSector = i;
		for(size_t j = 0; j < size; j++) {
			if(map[j + i] == true) {
				beginSector = 0;
				break;
			}
		}
		if(beginSector != 0)
			break;
	}
	//unused space wasn't found or filename repeats
	if(beginSector == 0) {
		return 1;
	}
	//set file's sectors as used
	for(size_t i = beginSector; i < size + beginSector; i++)
		map[i] = true;

	files[numberOfFiles].beginSector = beginSector;
	files[numberOfFiles].size = size;
	files[numberOfFiles].track = 0;
	strncpy(files[numberOfFiles].name, str, 16);
	numberOfFiles++;

	//copy new files array to memory
	for(size_t i = 0; i < numberOfFiles; i++) {
		memncpy((char *)(FAT + 3 + i * 19), (char *)(files + i), 19);
	}

	//save it
	asm("mov es,si\n"
	    "xor ch, ch\n"
	    "mov ah, 3\n"
	    "int 0x13"
	    ::"a"(1), "d"(0), "c"(2), "b"(0), "S"(KERNEL_ADDRESS));
	return 0;
}

__attribute__((interrupt))
void int0x21(struct interruptFrame * frame) {
	//see interrupts.asm
	//DS and CS are on kernel
	//TODO: standarize error returns
	asm("push ds\nmov ds, ax"::"a"(KERNEL_ADDRESS));

	// registers are backed up, after this function, they are restored
	int ax = 0, bx = 0, cx = 0, dx = 0, di = 0, si = 0;
	asm("mov ax,[ebp-24]":"=a"(ax));
	asm("mov dx,[ebp-20]":"=d"(dx));
	asm("mov cx,[ebp-16]":"=c"(cx));
	asm("mov bx,[ebp-12]":"=b"(bx));
	asm("mov si,[ebp-8]":"=S"(si));
	asm("mov di,[ebp-4]":"=D"(di));
	switch(ax >> 8) {
	case 1:
		asm("mov [ebp-24], ax"::"a"(sys_fopen(bx)));
		break;
	case 2:
		asm("mov [ebp-24], ax"::"a"(sys_read(bx, cx, dx)));
		break;
	case 3:
		sys_write(bx, cx);
		break;
	case 4:
		asm("mov [ebp-24], ax"::"a"(sys_create(bx, cx)));
		break;
	default:
		printf("INT 0x21!\n");
		break;
	}
	asm("pop ds");
}
#pragma GCC diagnostic pop

#endif