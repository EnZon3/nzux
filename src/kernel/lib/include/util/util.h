//Includes and other definitions here
#include <stdint.h>
#include <stddef.h>

//Headers here

void outb(uint16_t port, uint8_t val);
char inb(uint16_t port);
void io_wait();
void* memset(void* bufptr, int value, size_t size);

struct InterruptRegisters {
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};