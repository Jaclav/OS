//TODO rename it to not mismatch with std
#ifndef STDLIB_H
#define STDLIB_H

#include "types.h"

Word getMemorySize(void) {
	// size in kB
	Word size;
	asm("int 0x12"
	    :"=a"(size));
	return size;
}

void udelay(unsigned long usecs) {
	//http://vitaly_filatov.tripod.com/ng/asm/asm_026.13.html
	asm("int 0x15"
	    :
	    :"a"(0x8600), "c"(usecs>>16), "d"((Word)usecs));
}

void exit(int val) {
	asm("push cx\n"
	    "push bx\n"
	    "push dx\n"
	    "iret"::"a"(val), "d"(*(int*)(0x8)), "b"(0x1000), "c"(0));
}

#endif