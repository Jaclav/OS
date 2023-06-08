#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <types.h>

struct Mouse {
	Byte x, y, status;
};
typedef struct Mouse Mouse;

enum CursorStatus {Nothing = 8, LeftPress = 9, RightPress = 10};

extern void mouse_start();
extern short mouseX, mouseY;
extern char curStatus;

int initialize() {

}

int setMouse() {

}

#endif //MOUSE_HPP
