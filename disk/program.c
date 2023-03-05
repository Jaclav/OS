#include "../kernel/interrupts.h"
#include "../kernel/io.h"
#include "../kernel/string.h"
#include "../kernel/fs.h"

char a[] = "Greetings from C disk!\n";
__attribute__((section("start")))
__attribute__((interrupt))//will return by iret - to offset AND segment
void main(struct interruptFrame* frame) {
	puts(a);
	Byte disk[512]={0};
	memset(disk, ' ', 512);
	if(readSector(disk, 17, 1) == 0) {
		puts("ERROR");
	}
	for(int i = 0; i < 512; i++)
		putc(disk[i]);
	putc('\n');
	return;
}
char b[] = "DISK 1234567890";
