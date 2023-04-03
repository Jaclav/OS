#include <io.h>
#include <string.h>
#include "../kernel/fs.h"//! remove, kernel only

char a[] = "\nGreetings from C disk!\n";
__attribute__((section("start")))
int main() {
	Byte parameters = 0x80;
	puts(parameters);
	//TODO: printf doesn't work printf("%s",a);
	puts(a);
	Byte disk[512];
	memset(disk, 'X', 512);

	FILE *file = NULL;
	file = fopen("data.txt", "");
	if(file == NULL) {
		puts("ERROR file connot be opened\n");
		return 404;
	}
	puti(file->beginSector);
	putc(':');
	puti(file->size);

	if(readSector(disk, file->beginSector, file->size) != file->size) {
		puts("ERROR file cannot be readed\n");
		return 403;
	}
	for(int i = 0; i < 512; i++)
		putc(disk[i]);
	putc('\n');

	return 1519;
}
char b[] = "DISK 1234567890";
