/**
 * @brief Remove $1 file
 */

__attribute__((section("start")))
int main() {
	if(*(char *)0x80 == 0)
		return -1;
	int ret = 0;
	asm("int 0x21":"=a"(ret):"a"(0x0500), "b"(0x80));

	asm("int 0x20"::"a"('\n'));//newline

	return ret;
}