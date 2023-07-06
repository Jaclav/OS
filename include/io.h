/**
 * @file io.h
 * @brief Standard input output library
 * @todo add cursorColor and setCursorColor(Color color)
 */
#ifndef IO_H
#define IO_H

#include "types.h"

/**
 * @brief Key information, got from buffer
 */
typedef union Key {
	struct {
		Byte character: 8;
		Byte scancode: 7;
		Byte available: 1;
	};
} Key;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * @brief Put character
 *
 * @param character
 */
void putc(Byte character) {
	asm("xor ax, ax\n"
	    "mov al, [ebp+8]\n"
	    "int 0x20");
}

/**
 * @brief Put string
 *
 * @param string
 */
void puts(const int string) {
	asm("mov bx,[ebp+8]\n"
	    "int 0x20"::"a"(0x100));
}

/**
 * @brief Put int
 * @details will put signed integer,  to use uint see %u in printf()
 * @param num
 */
void puti(int num) {
	asm("cmp dx, 0\n"
	    "jge L%=\n"
	    "mov ax, '-'\n"
	    "int 0x20\n"
	    "neg dx\n"
	    "L%=:\n"
	    "mov bx, dx\n"
	    "mov ah, 2\n"
	    "int 0x20\n"::"d"(num));
}

/**
 * @brief C-like printf function
 * @details See https://cplusplus.com/reference/cstdio/printf/
 *
 * @param str string to be printed with specifiers
 * @param ... additional arguments depending on specifiers
 * @bug %s cannot be string literal like "something" as argument in ...
 */
void printf(const int str, ...) {
	asm("lea bx, [ebp+8]\n"//BX = EBP + 8
	    "int 0x20"::"a"(0x0300));
	return;
}

/**
 * @brief Wait for key in buffer, get it and clear buffer
 *
 * @return Key
 */
Key getc(void) {
	Key key;
	asm("int 0x16"
	    :"=a"(key)
	    :"a" (0x0));
	return key;
}

/**
 * @brief Get key from buffer
 * @details to clear buffer use getc
 * @return Key from buffer
 */
Key getKeyBuff(void) {
	Key key;
	asm("int 0x16\n"
	    "mov bx, 0\n"
	    "jz .end\n"
	    "mov bx, 1\n"
	    ".end:"
	    :"=a"(key), "=b"(key.available)
	    :"a" (0x0100));
	return key;
}

#pragma GCC diagnostic pop
#endif