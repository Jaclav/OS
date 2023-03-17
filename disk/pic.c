#include <io.h>
#include <graphics.h>
#include "../kernel/fs.h"//! remove, kernel only

__attribute__((section("start")))
int main() {
	Byte name = 0x80;//name as parameter
	setVideoMode(0x13);
	puts("PIC\n");
	unsigned int image_width = 0x20;
	unsigned int image_height = 0x20;
	Color image_bmp[1024];
	Position pos = {100, 50};

	FILE *file;
	file = fopen(name, "");
	puts(name);

	if(readSector(image_bmp, file->beginSector, file->size) == 0) {
		puts("ERROR");
	}
	draw(pos, image_bmp, image_width, image_height);
	Key k = getc();//without KEY it would be removed by -Os
	setVideoMode(0x2);
	return 0;
}
