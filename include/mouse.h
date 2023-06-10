/**
 * @file mouse.h
 * @brief Get mouse position and status
 * @todo import all from disk/test/ms.asm
 */
#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <types.h>

typedef enum {NothingPress = 8, LeftPress = 9, RightPress = 10} CursorStatus;

typedef struct Mouse {
	Byte x, y;
	CursorStatus status;
} Mouse;

extern void mouse_start();
extern short mouseX, mouseY;
extern char curStatus;

/*int initialize() {

}

int setMouse() {

}*/

#endif //MOUSE_HPP
