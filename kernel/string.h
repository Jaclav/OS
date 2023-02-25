#include <stdbool.h>
#include "stdlib.h"

bool strcmp(char *str1, char *str2);
bool strncmp ( const char * str1, const char * str2, size_t num );
const char * strchr ( const char * str, int character );
char *strncpy ( char * destination, const char * source, size_t num );
size_t strlen(char *str);
int stoi(char *str);
void reset(char *str);