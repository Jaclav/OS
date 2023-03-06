#include <io.h>
#include <string.h>
#include <stdlib.h>
#include "../kernel/fs.h"//! remove, kernel only

char a[] = "Greetings from C disk!\n";
__attribute__((section("start")))
void main(struct interruptFrame* frame) {
	puts(a);
	/*Byte disk[512]={0};
	memset(disk, ' ', 512);
	if(readSector(disk, 18, 1) == 0) {
		puts("ERROR");
	}
	for(int i = 0; i < 512; i++)
		putc(disk[i]);
	putc('\n');*/
	asm volatile("ret");
	asm("xchg bx,bx");
	return;					//TODO: I don't know why compiler uses in every file retw i.e. 66c3 instead normal ret i.e. c3 - C program doesn't return properly
}
char b[] = "DISK 1234567890";
