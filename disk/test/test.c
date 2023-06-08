#include <io.h>
#include <file.h>
#include <conio.h>
#include <mouse.h>
#include <graphics.h>
#include <string.h>
#include <stdlib.h>

__attribute__((section("start")))
int main() {
	char a[] = "Greetings from C disk! Parameters:";
	Byte parameters = 0x80;
	printf("%s\"%s\"\n", a, parameters);
	Byte disk[512];

	//read and print file data.txt
	FILE *file = NULL;
	file = open("data.txt", "");
	if(file == NULL) {
		puts("ERROR file connot be opened\n");
		return 404;
	}
	size_t size = read(disk, 12, file);
	printf("Readed: %iB: ", size);
	for(size_t i = 0; i < size; i++)
		putc(disk[i]);
	printf("\nSize: %i; ID: %i\n", size, file->id);

	size = read(disk, 512, file);
	if(size != 512) {
		puts("ERROR file cannot be readed\n");
		return 403;
	}
	for(size_t i = 0; i < size; i++)
		putc(disk[i]);
	putc('\n');

	write("Hello!!! data.txt, I've overwritten it!", 40, file);
	create("HELLO.txt", 2);

	getc();
	// test colors
	for(int i = 0; i < 16; i++) {
		for(int j = 0; j < 16; j++)
			cputc('X', j + i * 16, 2);
		putc('\n');
	}
	//test interrupts
	asm("int 0x20"
	    ::"a"(0xff00));
	asm("int 0x21"
	    ::"a"(0xff00));

	//test string operations
	char L0[8] = "123456";
	char L1[] = "Napis1\n";
	puts(L1);
	printf("Napis2\npis456=%s\n", strncpy(L0, strchr(L1, 'p'), 3));

	char test2[] = "cdYou shouldn't see this";
	char test1[] = "ab";
	strcpy(test2, test1);
	printf("ab=%s", test2);

	//test graphics
	getc();
	setVideoMode(0x10);
	Position pos = {0, 0};

	for(int i = 0; i < 10000; i++) {
		pos.x += i;
		pos.y = i % 250;
		pos.x %= 250;
		writePixel(pos, Green);
	}

	for(size_t i = 0; i < SIZE_X; i++) {
		pos.x = i;
		pos.y = SIZE_Y - i;
		writePixel(pos, Cyan);
	}
	Key key;
	mouse_start();
	Color color = Red;
	do {
		printf("%i %i %i %i", mouseX, mouseY, curStatus, (Color)color);
		Position pos = {mouseX, mouseY};
		if(curStatus == LeftPress) {
			color--;
			udelay(100000);
		}
		if(curStatus == RightPress) {
			color++;
			udelay(100000);
		}
		writePixel(pos, color);

		key = getKeyBuff();
		if(key.available == 1) {
			key = getc();
			putc(key.character);
		}
		else
			putc('x');
		printf("      \r");
		udelay(10000);
	} while(key.character != 'a');
	setVideoMode(0x2);
	setColorPalette(DarkGrey);

	return -1234;
}
char b[] = "DISK 1234567890";