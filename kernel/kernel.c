// https://grandidierite.github.io/bios-interrupts/
// https://cplusplus.com/reference/cstdio/putchar/
// https://archive.kernel.org/oldlinux/htmldocs/kernel-api/API-memset.html
// https://en.wikipedia.org/wiki/INT_16H
// https://en.wikipedia.org/wiki/INT_10H
// https://en.wikipedia.org/wiki/INT_13H
// http://www.brackeen.com/vga/basics.html#3
//BUG: cannot give string literal to char* parameter, only int WHY!?
//TODO: handle key, shift, ctrl
//TODO: https://cplusplus.com/reference/cerrno/errno/ https://cplusplus.com/reference/cstring/strerror/ https://cplusplus.com/reference/system_error/errc/
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <graphics.h>
#include "interrupts.h"
#include "fs.h"

#ifndef KERNEL_ADDRESS
#define KERNEL_ADDRESS 0
#endif

extern int load(char beginSector, int parameter, int size);
int gets(char *str, int size);

FILE files[30];//TODO: update it at every file saving, do it dynamically
size_t numberOfFiles = 0;

__attribute__((interrupt))
void int0x21(struct interruptFrame* frame) {
	//see interrupts.asm
	asm("push ds\nmov ds, ax"::"a"(KERNEL_ADDRESS));

	int ax = 0, dx = 0;
	asm("mov ax,[ebp-16]":"=a"(ax));
	asm("mov bx,[ebp-4]");
	asm("mov cx,[ebp-8]");
	asm("mov dx,[ebp-12]":"=d"(dx));
	switch(ax >> 8) {
	case 1: {
		//copy string from dx to fileName
		char fileName[FILENAME_MAX];
		memset(fileName, 0, FILENAME_MAX);
		asm("mov si, dx\n\
mov di, ax\n\
loop%=:\n\
mov ax, ss:[si+bx]\n\
cmp ax,0\n\
je after%=\n\
mov byte ptr ds:[di+bx], al\n\
inc bx\n\
cmp bx,cx\n\
jl loop%=\n\
after%=:"
		    :: "a"(fileName), "b"(0), "c"(FILENAME_MAX), "d"(dx));
		for(int i = 0; i < numberOfFiles; i++) {
			if(strncmp(files[i].name, fileName, FILENAME_MAX)) {
				asm("mov [ebp-16],ax\nmov [ebp-4],bx"::"a"(files[i].beginSector), "b"(files[i].size));
				asm("pop ds");
				return;
			}
		}
		asm("mov [ebp-16],ax\nmov [ebp-4],bx"::"a"(0), "b"(0));
		break;
	}
	default:
		printf("INT 0x21!\n");
		break;
	}
	asm("pop ds");
}

__attribute__((section("start")))
void main() {
	setVideoMode(0x02);
	setColorPalette(VGA_COLOR_DARK_GREY);
	setInterrupts();
	addInterrupt(0x0021, int0x21);
	puts("Kernel loaded.\nVersion: "__DATE__" "__TIME__);
	printf("\nMemory size: %ikB\n>", getMemorySize());

	Byte *FAT = 0x0;//first 512 Bytes is file table
	if(FAT[0] != 0xcf || FAT[1] != 0xaa || FAT[2] != 0x55)
		puts("ERROR: wrong FAT table format!");
	for(; (FAT[numberOfFiles * 18 + 3] != 0) && numberOfFiles * 18 + 3 < 512; numberOfFiles++) {
		strncpy((char *)(files + numberOfFiles), (char *)FAT + 3 + numberOfFiles * 18, 18);
	}

	char buffor[100];
	int bufforSize = 0;

	char command[100];
	int commandSize = 0;

	char parameter[100];

	for(;;) {
		bufforSize = gets(buffor, 98);
		commandSize = (int)strchr(buffor, ' ') - (int)buffor;//when no parameters commandSize=-buffor, then just copy buffor
		if(commandSize < 0) {
			strcpy(command, buffor);
			parameter[0] = 0;
		}
		else {
			strncpy(command, buffor, commandSize);
			command[commandSize] = 0;//set null character
			strcpy(parameter, strchr(buffor, ' ') + 1);//copies also null
		}

		char CLS[] = "cls";
		char POS[] = "pos";
		char KEY[] = "key";
		char MODE[] = "mode";
		char TEST[] = "test";
		char LOAD[] = "load";
		char SEC[] = "sec";
		char LS[] = "ls";
		if(bufforSize == 0) {

		}
		else if(strcmp(command, CLS)) {
			cls();
		}
		else if(strcmp(command, POS)) {
			Position position = getCursorPosition();
			printf("%i:%i", position.x, position.y);
		}
		else if(strcmp(command, KEY)) {
			Key key = getc();
			printf("%i:%i", key.character, key.scancode);
		}
		else if(strcmp(command, MODE)) {
			setVideoMode(stoi(parameter));
			printf("Mode: %i", stoi(parameter));
		}
		else if(strcmp(command, TEST)) {
			for(int j = 1; j < 15; j++)
				cputc('X', j, 2);
			putc('\n');
			asm("int 0x20":
			    :"a"(0xff00));
			asm("int 0x21");
			printf("%s\n%s\n%s\n", buffor, command, parameter);
			char L0[8] = "123456";
			char L1[] = "Napis1\n";
			puts(L1);
			puts("Napis2\n");
			puts(strncpy(L0, strchr(L1, 'p'), 3));

			putc('\n');
			char test2[] = "cdYou shouldn't see this";
			char test1[] = "ab";
			strcpy(test2, test1);
			puts(test2);

			getc();
			setVideoMode(0x10);
			Position pos = {0, 0};

			for(int i = 0; i < 10000; i++) {
				pos.x += i;
				pos.y = i % 250;
				pos.x %= 250;
				writePixel(pos, VGA_COLOR_GREEN);
			}

			for(size_t i = 0; i < SIZE_X; i++) {
				pos.x = i;
				pos.y = SIZE_Y - i;
				writePixel(pos, VGA_COLOR_LIGHT_CYAN);
			}
			getc();
			setVideoMode(0x2);
			setColorPalette(VGA_COLOR_DARK_GREY);
		}
		else if(strcmp(command, LOAD)) {
			puti(load(stoi(parameter), parameter, 3));
		}
		else if(strcmp(command, SEC)) {
			/* read sector to table and display this table*/
			char disk[512];
			memset(disk, ' ', 512);
			if(readSector(disk, stoi(parameter), 1) == 0) {
				puts("ERROR");
			}
			for(int i = 0; i < 512; i++)
				putc(disk[i]);
			putc('\n');
		}
		else if(strcmp(command, LS)) {
			size_t i = 0;
			puts("NAME          SECTOR  SIZE\n");
			for(; i < numberOfFiles; i++) {
				puts(files[i].name);
				for(size_t j = 0; j < FILENAME_MAX - strlen(files[i].name); j++)putc(' ');
				printf("%i     %i\n", files[i].beginSector, files[i].size);
			}
			printf("%i file(s)\n", i);
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

int gets(char *str, int size) {
	Key key;
	int ptr = 0;
	int oldPtr = 0;
	for(; ptr < size;) {
		key = getc();
		if(key.character == 13)
			break;
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