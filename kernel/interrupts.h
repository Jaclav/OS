/**
 * @file interrupts.h
 * @brief Setting system interruptions, see interrupts.asm
 * @details See more: https://grandidierite.github.io/bios-interrupts/
 * @details https://en.wikipedia.org/wiki/INT_16H
 * @details https://en.wikipedia.org/wiki/INT_10H
 * @details https://en.wikipedia.org/wiki/INT_13H
 * @details http://www.brackeen.com/vga/basics.html#3
 * @details https://www.ctyme.com/rbrown.htm
 * @details https://wiki.osdev.org/Interrupt_Vector_Table
 * @details https://wiki.osdev.org/Interrupt_Service_Routines
 */
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"

extern int setInterrupts();
extern int addInterrupt(int number, int function);

/**
 * @brief frame for interruption, required by gcc for __int
 * @todo IDK how to use them, they are required by GCC to use __int
 */
typedef struct interruptFrame {
	Word ip;
	Word cs;
	Word flags;
	Word sp;
	Word ss;
} interruptFrame;

#define __int __attribute__((interrupt))

#endif