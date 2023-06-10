/**
 * @file string.h
 * @brief Simple string handling
 */
#ifndef STRING_H
#define STRING_H

#include "types.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
/**
 * @brief Compares two string until \0 appeares
 *
 * @param str1
 * @param str2
 * @return true if are same
 * @return false if not
 */
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

/**
 * @brief Returns length of string
 *
 * @param str
 * @return size_t
 */
size_t strlen(const char *str) {
	asm("xor eax,eax\n"
	    "L%=:\n"
	    "	cmp BYTE ptr [si+bx], 0\n"
	    "	je end%=\n"
	    "	inc bx\n"
	    "	jmp L%=\n"
	    "end%=:\n"
	    "	mov eax, ebx"::"b"(0), "S"(str));
}

#pragma GCC diagnostic pop

/**
 * @brief Find first appearance of character
 *
 * @param str
 * @param character
 * @return pointer to first appearance or NULL if not found
 */
const char * strchr ( const char * str, int character ) {
	while(*str != 0) {
		if(*str == character)
			return str;
		str++;
	}
	return NULL;
}

const uint32_t pows[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
/**
 * @brief Convert string to int
 *
 * @param str
 * @return int
 */
int stoi(const char *str) {
	int num = 0;
	size_t size = strlen(str) - 1;
	for(size_t i = (str[0] == '-' ? 1 : 0); str[i] != 0; i++)
		num += pows[size - i] * (str[i] - '0');

	return (str[0] == '-') ? -num : num;
}

/**
 * @brief Set count Bytes of *ptr to value
 *
 * @param ptr
 * @param value
 * @param count
 * @return *ptr
 */
void *memset(void * ptr, int value, size_t count) {
	asm("L%=:\n\
        mov [bx],dx\n\
        inc bx\n\
        loop L%="
	    ::"b"(ptr), "c"(count), "d"(value));
	return ptr;
}

/**
 * @brief Copy from source to destination until \0 appears
 *
 * @param destination
 * @param source
 * @return *destination
 */
char * strcpy ( char * destination, const char * source ) {
	//copy all characters, including null
	int i = 0;
	do {
		destination[i] = source[i];
		i++;
	} while(source[i - 1] != 0);
	return destination;
}

/**
 * @brief Copy num Bytes from source to destination until \0 appears
 *
 * @param destination
 * @param source
 * @param num
 * @return *destination
 */
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