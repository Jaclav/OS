char a[] = "Greetings from C disk!\n";
__attribute__((section("start")))
void main() {
	asm("int 0x20"::"a"(0x0100),"b"((int)a));
	asm("mov esp,ebp\npop ebp\niret");
	return;
}
char b[] = "DISK 1234567890";
