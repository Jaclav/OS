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
#define FILES_MAX 38

//do it static - only this file should be able to use this
struct {
	char name[FILENAME_MAX];
	Byte beginSector;
	Byte size;
} files[FILES_MAX];

bool map[FILES_MAX];

size_t numberOfFiles = 0;
const Byte *FAT = 0x0;//first 512 Bytes is file table

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void loadFAT() {
	numberOfFiles = 0;
	if(FAT[0] != 0xcf || FAT[1] != 0xaa || FAT[2] != 0x55)
		puts("ERROR: wrong FAT table format!");
	for(; (FAT[numberOfFiles * 18 + 3] != 0) && numberOfFiles * 18 + 3 < 512; numberOfFiles++) {
		memncpy((char *)(files + numberOfFiles), (char *)(FAT + 3 + numberOfFiles * 18), 18);
		for(int i = 0; i < files[numberOfFiles].size; i++) {
			map[files[numberOfFiles].beginSector + i] = true;
		}
	}
}

__attribute__((interrupt))
void int0x21(struct interruptFrame * frame) {
	//see interrupts.asm
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
	//Return AX = beginSector BX = size of file from name [DX]
	case 1: {
		char fileName[FILENAME_MAX];
		strncpy(fileName, dx, FILENAME_MAX);

		for(size_t i = 0; i < numberOfFiles; i++) {
			if(strncmp(files[i].name, fileName, FILENAME_MAX)) {
				asm("mov [ebp-24],	ax\n"
				    "mov [ebp-12],	bx"
				    ::"a"(files[i].beginSector), "b"(files[i].size));
				asm("pop ds");
				return;
			}
		}
		// if wasn't found
		//TODO create file if wasn't found
		asm("mov word ptr [ebp-24], 0\n"
		    "mov word ptr [ebp-12], 0");
		break;
	}
	//Set [DI] = disk data from SI begin sector and CX size
	case 2: {
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
		    "mov ah, 2\n"
		    "int 0x13\n"
		    "jnc exit%=\n"
		    "mov ax, 0\n"
		    "exit%=:\n"
		    "	xor ah,ah\n"
		    "	mov [ebp-24], ax"
		    ::"a"(cx), "b"(di), "c"(si), "d"(0));
		break;
	}
	//Save [SI] to sector DI
	case 3: {
		Byte toSave[512];
		memset(toSave, 0, 512);
		strncpy((char *)toSave, si, 512);
		/*
		* AH = 03h
		* AL = Number of sectors to write
		* CH = Cylinder number (10 bit value, upper 2 bits in CL)
		* CL = Starting sector number
		* DH = Head number
		* DL = Drive number
		* ES:BX = Address of memory buffer
		*/
		asm("mov es,si\n"
		    "xor ch, ch\n"
		    "mov ah, 3\n"
		    "int 0x13"
		    ::"a"(1), "d"(0), "c"(di), "b"(toSave), "S"(KERNEL_ADDRESS));
		break;
	}
	//create file
	case 4: {
		//if sector is occupied, don't use it
		for(int i = di; i < di + cx && i < FILES_MAX; i++) {
			if(map[i] == true) {
				asm("mov word ptr [ebp-24], 1");
				asm("pop ds");
				return;
			}
		}

		files[numberOfFiles].beginSector = di;
		files[numberOfFiles].size = cx;
		strncpy(files[numberOfFiles].name, si, 16);
		numberOfFiles++;

		//copy new files array to memory
		for(size_t i = 0; i < numberOfFiles; i++) {
			memncpy((char *)(FAT + 3 + i * 18), (char *)(files + i), 18);
		}

		asm("mov es,si\n"
		    "xor ch, ch\n"
		    "mov ah, 3\n"
		    "int 0x13"
		    ::"a"(1), "d"(0), "c"(2), "b"(0), "S"(KERNEL_ADDRESS));
		asm("mov word ptr [ebp-24], 0");
		break;
	}
	default:
		printf("INT 0x21!\n");
		asm("pop ds");
		break;
	}
	asm("pop ds");
}
#pragma GCC diagnostic pop

#endif