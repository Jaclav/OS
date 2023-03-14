#include <io.h>
#include <string.h>
#include "../kernel/fs.h"//! remove, kernel only

char a[] = "Greetings from C disk!\n";
__attribute__((section("start")))
int main() {
	Byte parameters = 0x80;
	puts(parameters);
	//TODO: printf doesn't work
	puts(a);
	Byte disk[512] = {0};
	memset(disk, 'X', 512);
	if(readSector(disk, 18, 1) == 0) {
		puts("ERROR");
	}
	for(int i = 0; i < 512; i++)
		putc(disk[i]);
	putc('\n');
	return 1519;
}
char b[] = "DISK 1234567890";
