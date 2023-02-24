#include "io.h"

//TODO: cannot use func("string") (const literal) nor *p (pointer to literal), only array of chars

void main() {
    setVideoMode(2);
    setColor(VGA_COLOR_DARK_GREEN);
    char L1[] = "Kernel loaded.\nVersion: ";
    print(L1);

    printInt(1234);
    putchar(' ');
    char L2[] = __TIME__;
    print(L2);
    putchar('\n');
    putchar('>');

    Key key;
    char command[100];
    reset(command);
    int ptr = 0;
    for(;;) {
        key = getchar();
        if(key.character == 13) {
            putchar('\n');
            const char CLS[] = "cls";
            const char POS[] = "pos";
            if(strcmp(command, CLS)) {
                cls();
            }
            else if(strcmp(command,POS)){
                Position position = getCursorPosition();
                printInt(position.x);
                putchar(':');
                printInt(position.y);
            }

            ptr = 0;
            reset(command);
            putchar('>');
            continue;
        }
        putchar(key.character);
        command[ptr++] = key.character;
    }

    asm("hlt");
}