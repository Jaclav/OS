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
    if(*str == 0)
        return 0;
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

void reset(char *str) {
    for(int i = 0; str[i] != 0; i++) {
        str[i] = 0;
    }
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
    for(size_t i = 0; i < num; i++) {
        destination[i] = source[i];
    }
    return destination;
}

#endif