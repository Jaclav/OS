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

void cls(void) {
	asm("int 0x10\n\
        cls_l:\n\
        mov ax,0\n\
        int 0x20\n\
        loop cls_l\n\
        mov ax,0x0200\n\
        int 0x10"
	    :
	    : "a" (0x0200),
	    "b" (0x0),
	    "c" (0xffff),
	    "d" (0x0));
}

#endif