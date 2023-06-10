/**
 * @file mouse.h
 * @brief Get mouse position and status
 * @todo import all from disk/test/ms.asm
 */
#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <types.h>

typedef struct Mouse {
	Byte x, y, status;
} Mouse;

enum CursorStatus {NothingPress = 8, LeftPress = 9, RightPress = 10};

extern void mouse_start();
extern short mouseX, mouseY;
extern char curStatus;

/*int initialize() {

}

int setMouse() {

}*/

#endif //MOUSE_HPP
