// https://grandidierite.github.io/bios-interrupts/
// https://en.wikipedia.org/wiki/INT_16H
// https://en.wikipedia.org/wiki/INT_10H
// https://en.wikipedia.org/wiki/INT_13H
// http://www.brackeen.com/vga/basics.html#3
//BUG: cannot give string literal to char* parameter, only int WHY!?
//TODO:https://stackoverflow.com/questions/34420076/how-to-get-realtime-key-presses-in-assembly
#define KERNEL 1
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <graphics.h>
#include "interrupts.h"
#include "fs.h"

#ifndef KERNEL_ADDRESS
#define KERNEL_ADDRESS 0
#endif

extern int load(Byte beginSector, Byte track, int parameter, int size);
int gets(char *str, int size);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
__int void timer(struct interruptFrame * frame) {
//https://wiki.osdev.org/Processes_and_Threads
	static int a = 0;
	/*int ax = 0, bx;
	asm("mov ax,es\n"
	    "mov bx,ss":"=a"(ax), "=b"(bx));
	if(bx == 0x2000 && ax == 0x2000 && a % 1000 == 0) {
		// puti(a / 1000); why does it matter
		asm("mov		ax,		0x2000");
		asm("mov		ds,		ax");
		asm("mov		ss,		ax");
		asm("mov		es,		ax");
		asm("mov		fs,		ax");
		asm("mov		gs,		ax");

		DEBUG;
		exit(-1);
		asm("push cx\n"
		    "push bx\n"
		    "push dx\n"
		    "iret"::"a"(-1), "d"(*(int*)(0x8)), "b"(0x1000), "c"(0));
	}*/
	a++;
}
#pragma GCC diagnostic pop

__attribute__((section("start")))
void main() {
	setVideoMode(MODE_TEXT);
	setColorPalette(DarkGrey);
	setInterrupts();
	addInterrupt(0x0021, int0x21);
	addInterrupt(0x001c, timer);
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
		else {
			//check if there is program called command+".com"
			//TODO do it by chcecking if file exists by FILE fopen
			size_t i = 0;
			char com[] = ".com";
			strncpy(command + strlen(command), com, 5);
			for(; i < numberOfFiles; i++) {
				if(strcmp(files[i].name, command)) {
					retVal = load(files[i].beginSector, files[i].track, parameter, files[i].size);
afterCOM:
					if(retVal != 0) {
						cputs("Error:", Red);
						printf(" \"%s\" returned %i\n", command, retVal);
					}

					goto mainLoop;
				}
			}
			if(i == numberOfFiles) {
				cputs("Error:", Red);
				printf(" \"%s\" is unknown command!\n", command);
			}
		}
mainLoop:
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