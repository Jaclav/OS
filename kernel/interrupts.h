#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"

extern int setInterrupts();
extern int addInterrupt(int number, int function);

struct interruptFrame
{
    Word ip;
    Word cs;
    Word flags;
    Word sp;
    Word ss;
};
typedef struct interruptFrame interruptFrame;

#endif