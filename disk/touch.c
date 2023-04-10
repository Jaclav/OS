
//FIXME max size - 10
#define NULL 0
__attribute__((section("start")))
int main() {
	char* param1 = NULL;
	char* param2 = NULL;
	char *parameter = 0x80;
	for(int i = 0; parameter[i] != 0; i++) {
		if(parameter[i] == ' ') {
			parameter[i] = 0;
			param1 = parameter;
			if(parameter[i + 1] != 0)
				param2 = parameter + i + 1;
			break;
		}
	}
	int ret = 1;
	int val = param2[0] - '0';
	if(param1 != NULL && param2 != NULL)
		asm("int 0x21":"=a"(ret):"a"(0x0400), "c"(val), "S"(param1));

	return ret;
}