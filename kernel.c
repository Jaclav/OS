#include "kernel/io.h"

//TODO: cannot use func("string") (const literal) nor *p (pointer to literal), only array of chars

void abc() {
    putc('X');
}

__attribute__ ((section ("kernelMain")))
void main() {
    setVideoMode(0x02);
    setBackgroundColor(VGA_COLOR_DARK_GREEN);
    abc();
    //puts("TEST of string literals"); //TODO: this crashes, why!?
    char L1[] = "Kernel loaded.\nVersion: ";
    puts(L1);
    char L2[] = __TIME__;
    puts(L2);
    char L3[] = "\nMemory size: ";
    puts(L3);
    puti(getMemorySize());
    char L4[] = "kB\n>";
    puts(L4);

    char command[100];
    reset(command);
    int ptr = 0;
    for(;;) {
        ptr = gets(command);
        char CLS[] = "cls";
        char POS[] = "pos";
        char KEY[] = "key";
        char MODE[] = "mode";
        char TEST[] = "test";
        if(ptr==0){

        }
        else if(strcmp(command, CLS)) {
            cls();
        }
        else if(strcmp(command, POS)) {
            Position position = getCursorPosition();
            puti(position.x);
            putc(':');
            puti(position.y);
        }
        else if(strcmp(command, KEY)) {
            Key key = getc();
            puti(key.character);
            putc(':');
            puti(key.scancode);
        }
        else if(strcmp(command, MODE)) {
            gets(command);
            setVideoMode(stoi(command));
        }
        else if(strcmp(command, TEST)) {
            char L0[] = "123456";
            char L1[] = "Napis1\n";
            char L3[] = "Napis3\n";
            puts(L1);
            puts("Napis2\n");
            puts(L3);
            puti(pow(2, 0));
            putc(' ');
            puti(pow(2, 10));
            putc('\n');
            puti(strlen(L0));
            putc(' ');
            puti(stoi(L0));
            putc('\n');
            puti(2147483647);
        }
        else{
            char L0[]="Error: unknown command!\n";
            puts(L0);
        }
        putc('>');
    }

    asm("hlt");
}