#include "io.h"

void main() {
    setVideoMode(2);
    setColor(VGA_COLOR_DARK_GREEN);
    print("Kernel loaded.\nVersion: ");

    printInt(1234);
    putchar(' ');
    print(__TIME__);
    putchar('\n');

    for(;;) {

    }

    asm("hlt");
}
