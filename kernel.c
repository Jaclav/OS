#include "kernel/io.h"

//TODO: cannot use func("string") (const literal) nor *p (pointer to literal), only array of chars

void abc() {
    printChar('X');
}

__attribute__ ((section ("kernelMain")))
void main() {
    setVideoMode(0x02);
    setBackgroundColor(VGA_COLOR_DARK_GREEN);
    abc();
    //print("TEST of string literals"); //TODO: this crashes, why!?
    char L1[] = "Kernel loaded.\nVersion: ";
    print(L1);
    char L2[] = __TIME__;
    print(L2);
    char L3[] = "\nMemory size: ";
    print(L3);
    printInt(getMemorySize());
    char L4[] = "kB\n>";
    print(L4);

    char command[100];
    reset(command);
    int ptr = 0;
    for(;;) {
        ptr = getStr(command);
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
            printChar(':');
            printInt(position.y);
        }
        else if(strcmp(command, KEY)) {
            Key key = getKey();
            printInt(key.character);
            printChar(':');
            printInt(key.scancode);
        }
        else if(strcmp(command, MODE)) {
            getStr(command);
            setVideoMode(stoi(command));
        }
        else if(strcmp(command, TEST)) {
            char L0[] = "123456";
            char L1[] = "Napis1\n";
            char L3[] = "Napis3\n";
            print(L1);
            print("Napis2\n");
            print(L3);
            printInt(pow(2, 0));
            printChar(' ');
            printInt(pow(2, 10));
            printChar('\n');
            printInt(strlen(L0));
            printChar(' ');
            printInt(stoi(L0));
            printChar('\n');
            printInt(2147483647);
        }
        else{
            char L0[]="Error: unknown command!\n";
            print(L0);
        }
        printChar('>');
    }

    asm("hlt");
}