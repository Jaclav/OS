// https://grandidierite.github.io/bios-interrupts/
// https://cplusplus.com/reference/cstdio/putchar/
// https://archive.kernel.org/oldlinux/htmldocs/kernel-api/API-memset.html
// https://en.wikipedia.org/wiki/INT_16H
// https://en.wikipedia.org/wiki/INT_10H
// https://en.wikipedia.org/wiki/INT_13H
// http://www.brackeen.com/vga/basics.html#3
//BUG: cannot give string literal to char* parameter, only int WHY!?
//TODO: handle key, shift, ctrl
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <graphics.h>
#include "interrupts.h"

#ifndef KERNEL_ADDRESS
#define KERNEL_ADDRESS 0
#endif

extern int load(char beginSector, int parameter, int size);
void int0x21(struct interruptFrame* frame);
int gets(char *str, int size);

FILE files[30];//TODO: update it at every file saving, do it dynamically
size_t numberOfFiles = 0;
const Byte *FAT = 0x0;//first 512 Bytes is file table

__attribute__((section("start")))
void main() {
	setVideoMode(0x02);
	setColorPalette(VGA_COLOR_DARK_GREY);
	setInterrupts();
	addInterrupt(0x0021, int0x21);
	puts("Kernel loaded.\nVersion: "__DATE__" "__TIME__);
	printf("\nMemory size: %ikB\n>", getMemorySize());

	//load files from memory[0:0] to files array
	numberOfFiles = 0;
	if(FAT[0] != 0xcf || FAT[1] != 0xaa || FAT[2] != 0x55)
		puts("ERROR: wrong FAT table format!");
	for(; (FAT[numberOfFiles * 18 + 3] != 0) && numberOfFiles * 18 + 3 < 512; numberOfFiles++) {
		memncpy((char *)(files + numberOfFiles), (char *)(FAT + 3 + numberOfFiles * 18), 18);
	}

	int bufforSize = 0;
	char command[100];
	char parameter[100];

	for(;;) {
		bufforSize = gets(command, 98);
		if(strchr(command, ' ') == NULL)
			parameter[0] = 0;
		else {
			strcpy(parameter, strchr(command, ' ') + 1);
			*(char *)strchr(command, ' ') = 0;
		}

		char CLS[] = "cls";
		char POS[] = "pos";
		char KEY[] = "key";
		char MODE[] = "mode";
		char LOAD[] = "load";
		char SEC[] = "sec";
		char LS[] = "ls";
		char TOUCH[] = "touch";
		if(bufforSize == 0) {

		}
		else if(strcmp(command, CLS)) {
			cls();
		}
		else if(strcmp(command, POS)) {
			Cursor cursor = getCursorPosition();
			printf("%i:%i", cursor.x, cursor.y);
		}
		else if(strcmp(command, KEY)) {
			Key key = getc();
			printf("character=%i,scancode=%i, color=", key.character, key.scancode);
			int a = 0;
			asm("int 0x10":"=a"(a):"a"(0x0800), "b"(0x0000));
			puti(a >> 8);
		}
		else if(strcmp(command, MODE)) {
			setVideoMode(stoi(parameter));
			printf("Mode: %i", stoi(parameter));
		}
		else if(strcmp(command, LOAD)) {
			puti(load(stoi(parameter), parameter, 3));
		}
		else if(strcmp(command, SEC)) {
			/* read sector to table and display this table*/
			char disk[512];
			memset(disk, ' ', 512);
			FILE f;
			f.beginSector = stoi(parameter);
			fread(disk, 512, 1, &f);
			for(int i = 0; i < 512; i++)
				putc(disk[i]);
			putc('\n');
		}
		else if(strcmp(command, LS)) {
			size_t i = 0;
			size_t sum = 0;
			cputs("NAME          SECTOR     SIZE", VGA_COLOR_CYAN);
			putc('\n');
			for(; i < numberOfFiles; i++) {
				puts(files[i].name);
				for(size_t j = 0; j < FILENAME_MAX - strlen(files[i].name); j++)putc(' ');
				printf("%s%i        %s%i\n", (files[i].beginSector / 10 >= 1 ? "" : "0"), files[i].beginSector, ((files[i].size / 10) >= 1 ? "" : "0"), files[i].size);
				sum += files[i].size;
			}
			printf("\n%i file(s)       %s%i sector(s)\n", i, (sum / 10 >= 1 ? "" : "0"), sum);
		}
		else if(strcmp(command, TOUCH)) {
			if(strchr(parameter, ' ') != NULL) {
				char param[2][16];
				memset(param, 0, 2 * 16);
				strncpy(param[0], parameter, strchr(parameter, ' ') - parameter);
				char *ptr = (char *)strchr(parameter, ' ');
				*ptr = 0;
				ptr++;
				if(strchr(ptr, ' ') != NULL) {
					strncpy(param[1], ptr, strchr(ptr, ' ') - ptr);

					ptr = (char *)strchr(ptr, ' ');
					*ptr = 0;
					ptr++;
					create(param[0], stoi(param[1]), stoi(ptr));
				}
			}
		}
		else {
			//check if there is program called command+".com"
			size_t i = 0;
			char tmp[104];
			char com[] = ".com";
			strcpy(tmp, command);
			strcpy(tmp + strlen(tmp), com);
			for(; i < numberOfFiles; i++) {
				if(strcmp(files[i].name, tmp)) {
					int ret = load(files[i].beginSector, parameter, files[i].size);
					if(ret != 0) {
						cputs("Error:", VGA_COLOR_RED);
						printf(" \"%s\" returned %i\n", tmp, ret);
					}

					break;
				}
			}
			if(i == numberOfFiles) {
				cputs("Error:", VGA_COLOR_RED);
				printf(" \"%s\" is unknown command!\n", tmp);
			}
		}
		putc('>');
	}

	asm("hlt");
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

__attribute__((interrupt))
void int0x21(struct interruptFrame* frame) {
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
	//TODO make it safe!!
	case 4: {
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
		break;
	}
	default:
		printf("INT 0x21!\n");
		break;
	}
	asm("pop ds");
}
#pragma GCC diagnostic pop

int gets(char *str, int size) {
	Key key;
	int ptr = 0;
	int oldPtr = 0;
	for(; ptr < size;) {
		key = getc();
		if(key.character == 13) {
			str[ptr] = 0;
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
		else if(key.character == 0 && key.scancode == 72) {
			asm("int 0x10"::"a"(0x0601), "b"(0x0000), "c"(0x0000), "d"(0xffff));
			ptr = 0;
			continue;
		}
		else if(key.character == 0 && key.scancode == 80) {
			asm("int 0x10"::"a"(0x0701), "b"(0x0000), "c"(0x0000), "d"(0xffff));
			ptr = 0;
			continue;
		}
		else if(key.character == 0 && key.scancode == 75) {
			if(ptr > 0) {
				//move cursor by -1
				asm("int 0x10\n\
		    mov ah,02\n\
		    dec dl\n\
		    int 0x10"
				    ::"a" (0x0300), "b" (0x0));
				if(oldPtr < ptr)
					oldPtr = ptr;
				ptr--;
			}
			continue;
		}
		else if(key.character == 0 && key.scancode == 77) {
			if(ptr < oldPtr) {
				//move cursor by 1
				asm("int 0x10\n\
		    mov ah,02\n\
		    inc dl\n\
		    int 0x10"
				    ::"a" (0x0300), "b" (0x0));
				if(ptr < size) {
					ptr++;
					if(ptr == oldPtr)
						oldPtr = 0;
				}
			}
			continue;
		}
		putc(key.character);
		str[ptr++] = key.character;
	}
	str[oldPtr < ptr ? ptr : oldPtr] = 0; //end string with null
	putc('\n');
	return ptr;
}