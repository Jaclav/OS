#ifndef STRING_H
#define STRING_H

#include "types.h"
#include "math.h"

bool strcmp(char *str1, char *str2) {
	while(*str1 != 0 && *str2 != 0) {
		if(*str1 != *str2)
			return false;
		str1++;
		str2++;
	}
	if(*str1 != *str2)
		return false;
	return true;
}

bool strncmp ( const char * str1, const char * str2, size_t num ) {
	for(size_t i = 0; i < num; i++) {
		if(str1[i] != str2[i])
			return false;
		if(str1[i] == 0)
			break;
	}
	return true;
}

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
	for(size_t i = 0; i <= size; i++) {
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
	    "	mov al, byte ptr ss:[si+bx]\n"
	    "	mov byte ptr cs:[di+bx], al\n"
	    "	cmp al,0\n"
	    "	je after%=\n"
	    "	inc bx\n"
	    "loop L%=\n"
	    "after%=:"
	    ::"D"(destination), "S"(source), "b"(0), "c"(num));
	return destination;
}

char *memncpy ( char * destination, const char * source, size_t num ) {
	for(size_t i = 0; i < num; i++) {
		destination[i] = source[i];
	}
	return destination;
}

#endif