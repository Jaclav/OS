/**
 * @file stdlib.h
 * @brief Standard libary
 * @todo change name to not be mismatched with stdlib.h
 */
#ifndef STDLIB_H
#define STDLIB_H

#include "types.h"

/**
 * @brief Get RAM size in first 1MB
 * @todo get RAM size above first 1MB with int 0x15 AH=0x88, see info.com
 * @return size in kB
 */
Word getMemorySize(void) {
	Word size;
	asm("int 0x12"
	    :"=a"(size));
	return size;
}

/**
 * @brief Delay usecs microseconds
 *
 * @param usecs
 */
void udelay(unsigned long usecs);
void udelay(unsigned long usecs) {
	//http://vitaly_filatov.tripod.com/ng/asm/asm_026.13.html
	asm("int 0x15"
	    :
	    :"a"(0x8600), "c"(usecs>>16), "d"((Word)usecs));
}

/**
 * @brief Exit from program
 *
 * @param val return code
 * @todo to it as OS interruption
 */
void exit(int val) {
	asm("push cx\n"
	    "push bx\n"
	    "push dx\n"
	    "iretw"::"a"(val), "d"(*(int*)(0x8)), "b"(0x1000), "c"(0));
}

#endif