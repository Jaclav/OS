#include "kernel/interrupts.h"
#include "kernel/io.h"

char a[] = "Greetings from C disk!\n";
__attribute__((section("start")))
__attribute__((interrupt))//will return by iret - to offset AND segment
void main(struct interruptFrame* frame) {
	puts(a);
	return;
}
char b[] = "DISK 1234567890";
