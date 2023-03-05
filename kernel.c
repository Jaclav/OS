// https://grandidierite.github.io/bios-interrupts/
// https://cplusplus.com/reference/cstdio/putchar/
// https://archive.kernel.org/oldlinux/htmldocs/kernel-api/API-memset.html
// https://en.wikipedia.org/wiki/INT_16H
// https://en.wikipedia.org/wiki/INT_10H
// https://en.wikipedia.org/wiki/INT_13H
// http://www.brackeen.com/vga/basics.html#3
//TODO: cannot give string literal to char* parameter, only int WHY!?
//TODO: handle key, shift, ctrl
//TODO: https://cplusplus.com/reference/cerrno/errno/ https://cplusplus.com/reference/cstring/strerror/ https://cplusplus.com/reference/system_error/errc/
#include "kernel/io.h"
#include "kernel/string.h"
#include "kernel/interrupts.h"
#include "kernel/stdlib.h"
#include "kernel/graphics.h"
#include "kernel/fs.h"

#define KERNEL_ADDRESS 0x1000
#define DEBUG asm("xchg bx,bx");
extern int asmmain(char sector, int message);

__attribute__((interrupt))
void int0x21(struct interruptFrame* frame) {
	//see interrupts.asm
	asm("push ds\nmov ds, ax"::"a"(KERNEL_ADDRESS));
	puts("INT 0x21!\n");
	asm("pop ds");
}

__attribute__((section("start")))
void main() {
	puti(2137);
	setVideoMode(0x02);
	setColorPalette(VGA_COLOR_DARK_GREY);
	setInterrupts();
	addInterrupt(0x0021, int0x21);
	puts("Kernel loaded.\nVersion: ");
	puts(__DATE__);
	putc(' ');
	puts(__TIME__);
	puts("\nMemory size: ");
	puti(getMemorySize());
	puts("kB\n>");

	char buffor[100];
	int bufforSize = 0;

	char command[100];
	int commandSize = 0;

	char parameter[100];

	for(;;) {
		bufforSize = gets(buffor);
		commandSize = (int)strchr(buffor, ' ') - (int)buffor;//when no parameters commandSize=-buffor, then just copy buffor
		if(commandSize < 0) {
			strcpy(command, buffor);
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
		char DISK[] = "disk";
		char PIC[] = "pic";
		char SEC[] = "sec";
		if(bufforSize == 0) {

		}
		else if(strcmp(command, CLS)) {
			cls();
		}
		else if(strcmp(command, POS)) {
			Position position = getCursorPosition();
			puti(position.x);
			putc(':');
			puti(position.y);
		}
		else if(strcmp(command, KEY)) {
			Key key = getc();
			puti(key.character);
			putc(':');
			puti(key.scancode);
		}
		else if(strcmp(command, MODE)) {
			setVideoMode(stoi(parameter));
			puts("Mode: ");
			puti(stoi(parameter));
		}
		else if(strcmp(command, TEST)) {
			asm("int 0x20":
			    :"a"(0xff00));
			asm("int 0x21");
			puts(buffor);
			putc('\n');
			puts(command);
			putc('\n');
			puts(parameter);
			putc('\n');
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
			Position pos;

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
			gets(NULL);
			setVideoMode(0x2);
			setColorPalette(VGA_COLOR_DARK_GREY);
		}
		else if(strcmp(command, DISK)) {
			puti(asmmain(stoi(parameter), "Loading disk:"));
		}
		else if(strcmp(command, PIC)) {
			setVideoMode(0x13);
#include "image.h"
			Position pos = {100, 50};
			draw(pos, image_bmp, image_width, image_height);
		}
		else if(strcmp(command, SEC)) {
			char disk[512];
			memset(disk, ' ', 512);
			if(readSector(disk, stoi(parameter), 1) == 0) {
				puts("ERROR");
			}
			for(int i = 0; i < 512; i++)
				putc(disk[i]);
			putc('\n');
		}
		else {
			char L0[] = "Error: unknown command!\n";
			puts(L0);
		}
		putc('>');
	}

	asm("hlt");
}