#include <io.h>
#include <stdlib.h>

__attribute__((section("start")))
int main() {
	puts("SOMETHING!\n");
	int cx, dx, bx;
	asm("int 0x13":"=b"(bx), "=c"(cx):"a"(0x0800), "dl"(0));
	printf("CH=%i; CL=%i, BL=%i", cx >> 8, (Byte)cx, bx);
	exit(-1);
}