#ifndef STRING_H
#define STRING_H

#include "types.h"
#include "math.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
bool strcmp(int str1, int str2) {
	asm("L%=:\n"
	    "	mov al, [si+bx]\n"
	    "	cmp al, [di+bx]\n"
	    "	jne neq%=\n"
	    "	cmp al,0\n"
	    "	je eq%=\n"
	    "	inc bx\n"
	    "jmp L%=\n"
	    "eq%=:\n"
	    "	mov ax, 1\n"
	    "	jmp end%=\n"
	    "neq%=:\n"
	    "	mov ax, 0\n"
	    "end%=:"
	    ::"b"(0), "S"(str1), "D"(str2));
}
#pragma GCC diagnostic pop

const char * strchr ( const char * str, int character ) {
	while(*str != 0) {
		if(*str == character)
			return str;
		str++;
	}
	return NULL;
}

size_t strlen(const char *str) {
	size_t size = 0;
	for(; str[size] != 0; size++) {}
	return size;
}

int stoi(const char *str) {
	int num = 0;
	size_t size = strlen(str) - 1;
	for(size_t i = 0; str[i] != 0; i++) {
		num += pow(10, (size - i)) * (str[i] - '0');
	}
	return num;
}

void *memset(void * ptr, int value, size_t count) {
	asm("L%=:\n\
        mov [bx],dx\n\
        inc bx\n\
        loop L%="
	    ::"b"(ptr), "c"(count), "d"(value));
	return ptr;
}

char * strcpy ( char * destination, const char * source ) {
	//copy all characters, including null
	int i = 0;
	do {
		destination[i] = source[i];
		i++;
	} while(source[i - 1] != 0);
	return destination;
}

char *strncpy ( char * destination, const char * source, size_t num ) {
	asm("L%=:\n"
	    "	mov al, byte ptr [si+bx]\n"
	    "	mov byte ptr [di+bx], al\n"
	    "	cmp al,0\n"
	    "	je after%=\n"
	    "	inc bx\n"
	    "loop L%=\n"
	    "after%=:"
	    ::"D"(destination), "S"(source), "b"(0), "c"(num));
	return destination;
}

#endif