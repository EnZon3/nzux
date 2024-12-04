#ifndef _STRING_H
#define _STRING_H 1

//Includes and other definitions here
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//Headers here
void* memset(void* bufptr, int value, size_t size);
void* memcpy(void* restrict dest, const void* restrict src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
int memcmp(const void* aptr, const void* bptr, size_t n);
size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
int strcmp(const char* a, const char* b);
char *strtok(char *s, char *delim);

#ifdef __cplusplus
}
#endif

#endif