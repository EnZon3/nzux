#include "string.h"

int strcmp(const char *a, const char *b) {
    while(*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(const char*)a - *(const char *)b;
}