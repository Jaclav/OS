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

#ifndef KERNEL
#error Only kernel should use this file!
#endif

#include "types.h"

extern int setInterrupts();
extern int addInterrupt(int number, int function);

/**
 * @brief frame for interruption, required by gcc for __int
 * @todo IDK how to use them, they are required by GCC to use __int, see todo in interrupts.asm
 */
typedef struct interruptFrame {
	Word ip;
	Word cs;
	Word flags;
	Word sp;
	Word ss;
} interruptFrame;

#define __int __attribute__((interrupt))

/**
 * @brief Interruption flag
 */
#define IF 1<<9

/**
 * @brief End of interruption
 * @details See https://wiki.osdev.org/PIC#End_of_Interrupt
 * @details P0020003F RBIL ports.a
 * @details https://www.geeksforgeeks.org/operating-modes-of-8259-pic/
 * @details https://wiki.osdev.org/8259_PIC
 */
#define EOI() asm("out 0x20, al"::"a"(0x20));
#define REBOOT() asm("out 0x64, al"::"a"(0xfE));
#endif