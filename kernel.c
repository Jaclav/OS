#include "kernel/io.h"
#include "kernel/graphics.h"

//TODO: cannot use func("string") (const literal) nor *p (pointer to literal), only array of chars

extern void asmmain();

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

    char buffor[100];
    int bufforSize = 0;

    char command[100];
    int commandSize = 0;

    char parameter[100];
    for(;;) {
        bufforSize = gets(buffor);
        commandSize = (int)strchr(buffor, ' ') - (int)buffor;//when no parameters < 0, then just copy buffor
        strncpy(command, buffor, commandSize >= 0 ? commandSize : strlen(buffor) + 1);//copy also \0 character

        if((int)strchr(buffor, ' ') != 0) {
            strcpy(parameter, strchr(buffor, ' ') + 1);
        }

        char CLS[] = "cls";
        char POS[] = "pos";
        char KEY[] = "key";
        char MODE[] = "mode";
        char TEST[] = "test";
        char ASM[] = "asm";
        char GRAPHIC[] = "graphic";
        if(bufforSize == 0) {

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
            setVideoMode(stoi(parameter));
        }
        else if(strcmp(command, TEST)) {
            puts(buffor);
            putc('\n');
            puts(command);
            putc('\n');
            puts(parameter);
            putc('\n');
            char L0[8] = "123456";
            char L1[] = "Napis1\n";
            char L3[] = "Napis3\n";
            puts(L1);
            puts("Napis2\n");
            puts(L3);
            puts(strncpy(L0, strchr(L1, 'p'), 3));
        }
        else if(strcmp(command, ASM)) {
            asmmain();
        }
        else if(strcmp(command, GRAPHIC)) {
            setVideoMode(0x10);
            Position pos;

            for(int i = 0; i < 10000; i++) {
                pos.x += i;
                pos.y = i % 250;
                pos.x %= 250;
                writePixel(pos, VGA_COLOR_GREEN);
            }
            gets(NULL);
            setVideoMode(0x2);
        }
        else {
            char L0[] = "Error: unknown command!\n";
            puts(L0);
        }
        putc('>');
    }

    asm("hlt");
}