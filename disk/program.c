#include <io.h>
#include <string.h>
#include "../kernel/fs.h"//! remove, kernel only

char a[] = "Greetings from C disk!\n";
__attribute__((section("start")))
void main() {
	puts(a);
	Byte disk[512] = {0};
	memset(disk, 'X', 512);
	if(readSector(disk, 18, 1) == 0) {
		puts("ERROR");
	}
	for(int i = 0; i < 512; i++)
		putc(disk[i]);
	putc('\n');
	return;
}
char b[] = "DISK 1234567890";
