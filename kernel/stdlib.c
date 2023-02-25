#include "stdlib.h"

Word getMemorySize(void) {
    // size in kB
    Word size;
    asm("int 0x12"
        :"=a"(size));
    return size;
}
