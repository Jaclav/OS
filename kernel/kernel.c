/**
 * @file kernel.c
 * @author Jaclav
 * @brief Simple 16 bit kernel for BIOS
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 * @bug cannot give string literal to char* parameter, only int WHY!?
 * @bug cannot use * as pointers, int only
 */
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define KERNEL 1
#ifndef KERNEL_ADDRESS
#define KERNEL_ADDRESS 0
#endif

#include "interrupts.h"
#include "fs.h"

extern int load(Byte beginSector, Byte track, int parameter, int size);
int gets(char *str, int size);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
/**
 * @brief Interruption is called 18 times per second
 * @todo It should switch between processes, add multithreading
 * @bug doesn't work on bochs
 */
__int void timer(struct interruptFrame * frame) {
//https://wiki.osdev.org/Processes_and_Threads
	asm("push ds\nmov ds, ax"::"a"(KERNEL_ADDRESS));

	static int counter = 0;
	if(counter >= 1000) {
		putc('X');
		counter = 0;
		asm("out 0x20,al"::"a"(0xA0)); // https://wiki.osdev.org/PIC#End_of_Interrupt
		udelay(1000000);
		asm("mov		ds,		ax\n"
		    "mov		ss,		ax\n"
		    "mov		es,		ax\n"
		    "mov		fs,		ax\n"
		    "mov		gs,		ax\n"
		    "mov esp, 0xfff0\n"
		    "push cx\n"
		    "push ax\n"
		    "push dx\n"
		    "iret\n"::"a"(KERNEL_ADDRESS), "d"(0x200), "c"(0));
	}
	counter++;

	asm("pop ds");
}
#pragma GCC diagnostic pop

__start void main() {
	setVideoMode(TextMode);
	setColorPalette(DarkGrey);
	setInterrupts();
	addInterrupt(0x0021, int0x21);
	int bufforSize = 0;
	asm("int 0x21":"=a"(bufforSize):"a"(0));
	printf("Kernel loaded.\nVersion: "__DATE__" "__TIME__"\nMemory size: %ikB\nLoaded %i files\n>", getMemorySize(), bufforSize);

	char command[100];
	char parameter[100];
	int retVal;

	for(;;) {
		bufforSize = gets(command, 98);
		if(strchr(command, ' ') == NULL)
			parameter[0] = 0;
		else {
			strcpy(parameter, strchr(command, ' ') + 1);
			*(char *)strchr(command, ' ') = 0;
		}

		if(bufforSize == 0) {

		}
		else if(strcmp(command, "cls")) {
			cls();
		}
		else if(strcmp(command, "pos")) {
			Cursor cursor = getCursorPosition();
			printf("%i:%i", cursor.x, cursor.y);
		}
		else if(strcmp(command, "key")) {
			Key key = getc();
			Word special = 0;
			asm("int 0x16":"=a"(special):"a"(0x1200));
			printf("character=%i,scancode=%i,special=%b,color=", key.character, key.scancode, special);
			//get attribute
			int a = 0;
			asm("int 0x10":"=a"(a):"a"(0x0800), "b"(0x0000));
			puti(a >> 8);
		}
		else if(strcmp(command, "mode")) {
			setVideoMode(stoi(parameter));
			printf("Mode: %i", stoi(parameter));
		}
		else if(strcmp(command, "ls")) {
			size_t i = 0;
			size_t sum = 0;
			cputs("NAME         TRACK:SECTOR   SIZE", Cyan);
			putc('\n');
			for(; i < numberOfFiles; i++) {
				puts(files[i].name);
				for(size_t j = 0; j < FILENAME_MAX - strlen(files[i].name); j++)putc(' ');
				printf("%s%i:%s%i        %s%i\n", (files[i].track / 10 >= 1 ? "" : "0"), files[i].track, (files[i].beginSector / 10 >= 1 ? "" : "0"), files[i].beginSector, ((files[i].size / 10) >= 1 ? "" : "0"), files[i].size);
				sum += files[i].size;
			}
			printf("\n%i file(s)       %s%i sector(s)\n", i, (sum / 10 >= 1 ? "" : "0"), sum);
		}
		else if(strcmp(command, "map")) {
			for(int i = 1; i  < SECTORS_PER_TRACK; i++)putc((i % 10 + '0'));
			putc('\n');
			for(int j = 0; j < TRACKS_MAX; j++) {
				for(int i = 1; i < SECTORS_PER_TRACK; i++) {
					puti(map[j][i]);
				}
				putc('\n');
			}
		}
		else if(strcmp(command, "rm")) {
			if(parameter[0] == 0) {
				retVal = -1;
				goto afterCOM;//simulate rm as com command
			}
			asm("int 0x21":"=a"(retVal):"a"(0x0500), "b"(parameter));
			goto afterCOM;
		}
		else if(strcmp(command, "x")) {
			addInterrupt(0x001c, timer);
		}
		else {
			//check if there is program called command+".com"
			short i = 0;
			strncpy(command + strlen(command), ".com", 5);
			i = sys_open(command);
			if(i > 0) {
				i = (Byte)i;
				retVal = load(files[i].beginSector, files[i].track, parameter, files[i].size);
afterCOM:
				if(retVal != 0) {
					cputs("Error:", Red);
					printf(" \"%s\" returned %i\n", command, retVal);
				}
			}
			else {
				cputs("Error:", Red);
				if(i == -ENOENT)
					printf(" \"%s\" is unknown command!\n", command);
			}
		}
		putc('>');
	}

	asm("hlt");
}

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