#include <io.h>
#include <file.h>
#include <conio.h>
#include <string.h>

__attribute__((section("start")))
int main() {
	char data[2001];
	char view[80][25];
	memset(view, 0, 80 * 25);
	memset(data, 0, 2000);
	FILE* file = open(0x80, "");
	if(file == NULL)
		return -1;

	size_t size = read(data, 2000, file);

	cls();
	size_t n = 0;
	for(int j = 0; j < 24; j++) {
		for(int i = 0; i < 79; i++) {
			if(n > size)
				break;
			if(data[n] == '\n') {
				j++;
				i = -1;
				n++;
				continue;
			}
			view[i][j] = data[n];
			n++;
		}
	}
	Key key;
	Cursor crsr;
	Cursor def = getCursorPosition();
	int sizeY = 0;
	Attributes underCursor;
	underCursor.foreground = Black;
	underCursor.background = LightGrey;
	do {
		setCursorPosition(def);
		bool wasNewline;
		sizeY = 0;
		for(int j = 0; j < 24; j++) {
			wasNewline = false;
			for(int i = 0; i < 79; i++) {
				if(view[i][j] != 0) {
					if(i == crsr.x && j == crsr.y)
						cputc(view[i][j], underCursor.attributes, 1);
					else
						cputc(view[i][j], LightGrey, 1);
					wasNewline = true;
				}
				else if(i == crsr.x && j == crsr.y) {
					if(i != 0)
						while(crsr.x > 0 && view[crsr.x - 1][crsr.y] == 0)crsr.x--;
					cputc(view[i][j], underCursor.attributes, 1);
				}
			}
			if(!wasNewline)
				putc('~');
			else
				sizeY++;
			putc('\n');
		}
		printf("File: %s size: %i", 0x80, size);
		setCursorPosition(crsr);
		key = getc();
		if(key.character == 0 && key.scancode == 72) {//UP
			crsr = getCursorPosition();
			if(crsr.y > 0)
				crsr.y--;
			setCursorPosition(crsr);
		}
		else if(key.character == 0 && key.scancode == 80 ) {//DOWN
			crsr = getCursorPosition();
			if(crsr.y < sizeY)
				crsr.y++;
			if(crsr.y == sizeY)
				crsr.x = 0;
			setCursorPosition(crsr);
		}
		else if(key.character == 0 && key.scancode == 77) {//RIGHT
			crsr = getCursorPosition();
			crsr.x++;
			setCursorPosition(crsr);
		}
		else if(key.character == 0 && key.scancode == 75) {//LEFT
			crsr = getCursorPosition();
			crsr.x--;
			setCursorPosition(crsr);
		}
		else if(key.character == 8 && key.scancode == 14 && crsr.x > 0) { //backspace
			crsr = getCursorPosition();
			view[crsr.x ][crsr.y] = 0;
			crsr.x--;
			setCursorPosition(crsr);
		}
		else if(key.character == 13 && key.scancode == 28) {//enter
			crsr = getCursorPosition();
			if(crsr.y < sizeY)
				crsr.y++;
			crsr.x = 0;
			setCursorPosition(crsr);
		}
		else if(key.character == 19 && key.scancode == 31) {//save
			n = 0;
			bool isEmpty = true;
			cls();
			for(int j = 0; j < 24; j++) {
				isEmpty = true;
				for(int i = 0; i < 79; i++) {
					if(view[i][j] == 0)
						continue;
					isEmpty = false;
					data[n] = view[i][j];
					n++;
				}
				if(!isEmpty) {
					data[n] = '\n';
					n++;
				}
			}
			if(data[n - 1] == '\n')
				n--;

			printf("Saved %iB = %i sectors and %i Bytes\n", n, n / 512, n % 512);
			int ret = write(data, n, file);
			return (ret > 0 ? 0 : ret);
		}
		else {
			view[crsr.x][crsr.y] = key.character;//TODO: edit in data, not in view!!!
			crsr.x++;
		}
	}
	while(key.character != 27 && key.scancode != 1);//escape
	cls();
	return 0;
}