// https://grandidierite.github.io/bios-interrupts/
// https://cplusplus.com/reference/cstdio/putchar/
// https://archive.kernel.org/oldlinux/htmldocs/kernel-api/API-memset.html
// https://en.wikipedia.org/wiki/INT_16H
// https://en.wikipedia.org/wiki/INT_10H
// https://en.wikipedia.org/wiki/INT_13H
// http://www.brackeen.com/vga/basics.html#3
// https://www.ctyme.com/rbrown.htm
//TODO: cannot give string literal to char* parameter, only int
//TODO: handle key, shift, ctrl
//TODO: add file I/O
//TODO: read about wiki.osdev.org/Segmentation
#include "kernel/io.h"
#include "kernel/graphics.h"

extern int asmmain(char a, int b);
extern int setInterrupts();

void abc() {
	putc('X');
}

__attribute__ ((section ("kernelMain")))
void main() {
	setVideoMode(0x02);
	setColorPalette(VGA_COLOR_DARK_GREEN);
	setInterrupts();
	abc();
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
		char ASM[] = "asm";
		char GRAPHIC[] = "graphic";
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
			setColorPalette(VGA_COLOR_DARK_GREEN);
		}
		else if(strcmp(command, ASM)) {
			puti(asmmain('Y', "Loading disk:"));
		}
		else if(strcmp(command, GRAPHIC)) {
			setVideoMode(0x13);
#include "image.h"
			Position pos = {200, 100};
			draw(pos, image_bmp, image_width, image_height);
		}
		else {
			char L0[] = "Error: unknown command!\n";
			puts(L0);
		}
		putc('>');
	}

	asm("hlt");
}