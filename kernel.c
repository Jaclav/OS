#include "kernel/io.h"

//TODO: cannot use func("string") (const literal) nor *p (pointer to literal), only array of chars

void abc() {
    putchar('X');
}

__attribute__ ((section ("kernelMain")))
void main() {
    setVideoMode(0x02);
    setBackgroundColor(VGA_COLOR_DARK_GREEN);
    abc();
    //print("TEST of string literals"); //TODO: this crashes
    char L1[] = "Kernel loaded.\nVersion: ";
    print(L1);
    char L2[] = __TIME__;
    print(L2);
    char L3[] = "\nMemory size: ";
    print(L3);
    printInt(getMemorySize());
    char L4[] = "kB\n>";
    print(L4);

    Key key;
    char command[100];
    reset(command);
    int ptr = 0;
    for(;;) {
        key = getchar();
        if(key.character == 13) {//enter
            putchar('\n');
            char CLS[] = "cls";
            char POS[] = "pos";
            char KEY[] = "key";
            char MODE[] = "mode";
            char TEST[] = "test";
            if(strcmp(command, CLS)) {
                cls();
            }
            else if(strcmp(command, POS)) {
                Position position = getCursorPosition();
                printInt(position.x);
                putchar(':');
                printInt(position.y);
            }
            else if(strcmp(command, KEY)) {
                key = getchar();
                printInt(key.character);
                putchar(':');
                printInt(key.scancode);
            }
            else if(strcmp(command, MODE)) {
                key = getchar();
                setVideoMode(key.character - '0');
            }
            else if(strcmp(command, TEST)) {
                char L0[] = "123456";
                char L1[] = "Napis1\n";
                char L3[] = "Napis3\n";
                print(L1);
                print("Napis2\n");
                print(L3);
                printInt(pow(2, 0));
                putchar(' ');
                printInt(pow(2, 10));
                putchar('\n');
                printInt(strlen(L0));
                putchar(' ');
                printInt(stoi(L0));
            }

            ptr = 0;
            reset(command);
            putchar('>');
            continue;
        }
        else if(key.character == 8 && ptr >= 0) { //backspace
            if(ptr > 0) {
                command[ptr] = 0;
                ptr--;
                putchar(8);
                putchar(' ');
                putchar(8);
            }
            continue;
        }
        putchar(key.character);
        command[ptr++] = key.character;
    }

    asm("hlt");
}