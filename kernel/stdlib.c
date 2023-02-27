#include "stdlib.h"

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
        :"a"(0x86<<8), "c"(usecs>>16), "d"((Word)usecs));
}