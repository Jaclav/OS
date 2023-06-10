/**
 * @file touch.c
 * @brief Create file
 * @param filename
 * @param filesize
 */
#include <types.h>

__start int main() {
	char* param1 = NULL;
	char* param2 = NULL;
	for(int i = 0; * (char*)(0x80 + i) != 0; i++) {
		if(*(char*)(0x80 + i) == ' ') {
			*(char*)(0x80 + i) = 0;
			param1 = 0x80;
			if(*(char*)(0x81 + i) != 0)
				param2 = 0x81 + i;
			break;
		}
	}
	if(param2 == NULL)
		return -1;

	char pows[] = {1, 10, 100};
	size_t num = 0;
	size_t size = 0;
	for(; param2[size] != 0; size++) {}
	size--;

	for(size_t i = 0; param2[i] != 0; i++)
		num += pows[size - i] * (param2[i] - '0');

	int ret = 1;
	asm("int 0x21":"=a"(ret):"a"(0x0400), "b"(param1), "c"(num));

	return (ret > 0 ? 0 : ret);
}