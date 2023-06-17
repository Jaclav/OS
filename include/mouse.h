/**
 * @file mouse.h
 * @brief Get mouse position and status, to use mouse call mouseInitialize()
 * @details Inspired by https://stackoverflow.com/questions/54280828/making-a-mouse-handler-in-x86-assembly
 * @details See more: interrupts.c RBIL
 */
#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <types.h>
#include <errno.h>

#define HW_EQUIP_PS2 4
#define MOUSE_PKT_BYTES 3
#define MOUSE_RESOLUTION 3

typedef enum {NothingPress = 8, LeftPress = 9, RightPress = 10} MouseStatus;

/**
 * @brief Mouse values, changed by handler when mouse is enabled
 */
struct Mouse {
	short x, y, status;
} mouse;

/**
 * @brief Handler for mouse device
 * @details see M-15C207 in interrupts.c RBIL
 */
void mouseHandler() {
	char dx, dy;
	asm("pushw ds\n"
	    "pushw cs\n"
	    "popw ds\n");

	asm("mov ax, [ebp+10]\n"
	    "mov bx, [ebp+12]\n"
	    "mov cx, [ebp+14]":"=a"(dy), "=b"(dx), "=c"(mouse.status));
	mouse.x += dx;
	mouse.y -= dy;

	asm("popw ds\n"
	    "leave\n"
	    "retfw");
}

/**
 * @brief Disable mouse
 */
void mouseDisable() {
	asm("int 0x15\n"::"a"(0xc200), "b"(0));//Disable

	//clear callback function
	asm("pushw es\n"
	    "mov es, bx\n"
	    "int 0x15\n"
	    "popw es"::"a"(0xc207), "b"(0x0000));
}

/**
 * @brief Enable mouse
 */
void mouseEnable() {
	mouseDisable();

	//set callback function
	asm("pushw es\n"
	    "pushw cs\n"
	    "popw es\n"
	    "int 0x15\n"
	    "popw es"::"a"(0xc207), "b"(mouseHandler));
	asm("int 0x15\n"::"a"(0xc200), "b"(0x100));//Enable
}

/**
 * @brief Initialize mouse, by default mouse in enabled
 *
 * @return int 0 if success -ENODEV if error
 */
int mouseInitialize() {
	short installedDevices;
	asm("int 0x11":"=a"(installedDevices));
	if((installedDevices & HW_EQUIP_PS2) == 0)
		return -ENODEV;

	asm("int 0x15\n"
	    "jnc exit%=\n"
	    "mov ax, -1\n"
	    "exit%=:\n"
	    :"=a"(installedDevices):"a"(0xc205), "b"(MOUSE_PKT_BYTES<<8));
	if(installedDevices < 0)
		return -ENODEV;

	asm("int 0x15\n"
	    "jnc exit%=\n"
	    "mov ax, -1\n"
	    "exit%=:\n"
	    :"=a"(installedDevices):"a"(0xc203), "b"(MOUSE_RESOLUTION<<8));
	if(installedDevices < 0)
		return -ENODEV;

	mouseEnable();
	return 0;
}

#endif //MOUSE_HPP
