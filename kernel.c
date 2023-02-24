#include "io.h"

void main() {
    setVideoMode(3);
    setColor(VGA_COLOR_GREEN);
    puts("Kernel loaded.\n");

    for(;;){

    }

    asm("hlt");
}