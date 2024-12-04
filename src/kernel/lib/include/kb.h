#ifndef _KB_H
#define _KB_H 1

//Includes and other definitions here
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void kb_init();
void clear_txt();
void rm();
void append(char c);

#ifdef __cplusplus
}
#endif

#endif