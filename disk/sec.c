/**
 * @brief Write data from $1 track $2 sector
 */
typedef unsigned int size_t;
#define NULL 0

int stoi(const char* str) {
	char pows[] = {1, 10, 100};
	size_t num = 0;
	size_t size = 0;
	for(; str[size] != 0; size++) {}
	size--;

	for(size_t i = 0; str[i] != 0; i++)
		num += pows[size - i] * (str[i] - '0');
	return num;
}

__attribute__((section("start")))
int main() {
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

	int track = stoi(param1);
	int sector = stoi(param2);

	if(param2 != NULL) {
		char disk[512];
		asm("mov ah, 2\n"
		    "int 0x13\n"
		    ::"a"(1), "b"(disk), "c"(track<<8 | sector), "d"(0));
		for(int i = 0; i < 512; i++)
			asm("xor ah, ah\n"
			    "int 0x20"::"a"(disk[i]));

		asm("mov al, 10 \n"
		    "int 0x20"::"a"(0));
	}

	return 0;
}