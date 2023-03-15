#include <io.h>
#include <string.h>
#include "../kernel/fs.h"//! remove, kernel only

char a[] = "\nGreetings from C disk!\n";
__attribute__((section("start")))
int main() {
	Byte parameters = 0x80;
	puts(parameters);
	//TODO: printf doesn't work
	puts(a);
	Byte disk[512] = {0};
	memset(disk, 'X', 512);

	FILE *file;
	char name[] = "data.txt";
	file = fopen(name, "");
	puti(file->size);
	puti(file->beginSector);

	if(file == NULL) {
		puts("ERROR file connot be opened");
		return 404;
	}

	if(readSector(disk, file->beginSector, file->size) == 0) {
		puts("ERROR");
		return 403;
	}
	for(int i = 0; i < 512; i++)
		putc(disk[i]);
	putc('\n');

	return 1519;
}
char b[] = "DISK 1234567890";
