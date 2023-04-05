#include <io.h>
#include <graphics.h>
#include <string.h>

char a[] = "\nGreetings from C disk!\n";
__attribute__((section("start")))
int main() {
	//TODO: printf doesn't work printf("%s",a);
	Byte parameters = 0x80;
	puts(parameters);
	puts(a);
	Byte disk[512];

	//read and print file data.txt
	FILE *file = NULL;
	file = fopen("data.txt", "");
	if(file == NULL) {
		puts("ERROR file connot be opened\n");
		return 404;
	}
	puti(file->beginSector);
	putc(':');
	puti(file->size);

	if(fread(disk, 512, file->size, file) != file->size) {
		puts("ERROR file cannot be readed\n");
		return 403;
	}
	for(int i = 0; i < 512; i++)
		putc(disk[i]);
	putc('\n');

	fputs("Hello!!! data.txt, I've overwritten it!", file);
	create("HELLO.txt", 36, 2);

	// test graphics
	for(int j = 1; j < 15; j++)
		cputc('X', j, 2);
	putc('\n');
	asm("int 0x20":
	    :"a"(0xff00));
	asm("int 0x21");

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

	return 1519;
}
char b[] = "DISK 1234567890";
