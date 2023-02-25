#include "string.h"
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

int strlen(char *str) {
    if(*str == 0)
        return 0;
    int size = 0;
    for(; *str != 0; str++, size++) {}
    return size;
}

int stoi(char *str) {
    int num = 0;
    int size = strlen(str) - 1;
    for(size_t i = 0; i <= size; i++) {
        num += pow(10, (size - i)) * (str[i] - '0');
    }
    return num;
}

void reset(char *str) {
    while(*str != 0) {
        *str = 0;
        str++;
    }
}