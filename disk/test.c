#include <io.h>
#include <graphics.h>
#include <string.h>

__attribute__((section("start")))
int main() {
	char a[] = "Greetings from C disk! Parameters:";
	Byte parameters = 0x80;
	printf("%s %s\n", a, parameters);
	Byte disk[512];

	//read and print file data.txt
	FILE *file = NULL;
	file = fopen("data.txt", "");
	if(file == NULL) {
		puts("ERROR file connot be opened\n");
		return 404;
	}
	printf("Size: %i; ID: %i\n", file->size, file->id);

	if(fread(disk, 512, file->size, file) != file->size) {
		puts("ERROR file cannot be readed\n");
		return 403;
	}
	for(int i = 0; i < 512; i++)
		putc(disk[i]);
	putc('\n');

	fputs("Hello!!! data.txt, I've overwritten it!", file);
	create("HELLO.txt", 2);

	// test colors
	for(int j = 1; j < 15; j++)
		cputc('X', j, 2);
	putc('\n');

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

	return -1234;
}
char b[] = "DISK 1234567890";
