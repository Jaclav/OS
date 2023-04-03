#include <io.h>
#include <graphics.h>
#include "../kernel/fs.h"//! remove, kernel only

__attribute__((section("start")))
int main() {
	Byte name = 0x80;//name as parameter
	setVideoMode(MODE_COLOR);
	puts("PIC\n");
	unsigned int image_width = 0x20;
	unsigned int image_height = 0x20;
	Color image_bmp[1024];
	Position pos = {100, 50};

	FILE *file;
	file = fopen(name, "");
	if(file == NULL) {
		puts("ERROR file connot be opened\n");
		setVideoMode(MODE_TEXT);
		return 404;
	}
	puts(name);

	if(readSector(image_bmp, file->beginSector, file->size) != file->size) {
		puts("ERROR");
		setVideoMode(MODE_TEXT);
		return 403;
	}
	draw(pos, image_bmp, image_width, image_height);
	getc();//without KEY it would be removed by -Os
	setVideoMode(MODE_TEXT);
	return 0;
}
