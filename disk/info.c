/**
 * @file info.c
 * @brief Show extra memory, disk info, date and time
 * @date 2023-04-11
 *
 */
#include <io.h>

__attribute__((section("start")))
int main() {
	puts("\xc9\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcdINFO:\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xbb\n");
	Word ax, cx, dx, bx;
	asm("int 0x15":"=a"(ax):"a"(0x88), "b"(0));;
	printf("\xba\x45xtra memory: %ikB=%iMb\n", ax, ax / 1024);

	Byte ch, cl, dh, dl = 1;
	for(int i = 0; i < dl; i++) {
		asm("int 0x13":"=b"(bx), "=c"(cx), "=d"(dx):"a"(0x0800), "d"(i));
		ch = cx >> 8;
		cl = (Byte)cx;
		dh = dx >> 8;
		dl = (Byte)dx;
		int err = 0;
		asm("int 0x13":"=a"(err):"a"(0x0100));
		printf("\xba%i: tracks=%i S/T=%i type=%i H=%i disks=%i, err=%i\n", i, ch, cl, bx, dh, dl, err);
		asm("int 0x13"::"a"(0), "d"(i));//reset errors
	}

	asm("int 0x1a":"=d"(dx), "=c"(cx):"a"(0x0400));
	ch = cx >> 8;
	cl = (Byte)cx;
	dh = dx >> 8;
	dl = (Byte)dx;
	printf("\xba%i%i/%i/%i\n", (ch / 16) * 10 + ch % 16, (cl / 16) * 10 + cl % 16, (dh / 16) * 10 + dh % 16, (dl / 16) * 10 + dl % 16);

	asm("int 0x1a":"=d"(dx), "=c"(cx):"a"(0x0200));
	ch = cx >> 8;
	cl = (Byte)cx;
	dh = dx >> 8;
	dl = (Byte)dx;
	printf("\xba%i:%i:%i\n", (ch / 16) * 10 + ch % 16, (cl / 16) * 10 + cl % 16, (dh / 16) * 10 + dh % 16);
	puts("\xc8\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xbc\n");
	return 0;
}