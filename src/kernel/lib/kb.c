#include "util.h"
#include <stdbool.h>
#include "vga.h"
#include "kb.h"

bool shift = false;

void kb_handler(struct InterruptRegisters *r) {
    const char scancode = inb(0x60) & 0x7F;
    const char pressed = inb(0x60) & 0x80;

    // determine shift state
    if(scancode == 0x2A) {
        shift = !pressed;
    }
    
    if(scancode == 0x3A && !pressed) {
        shift = !shift;
    }

    //determine if printable character and if so print to screen
    if(pressed == 0 && keys_lower[scancode] >> 8 == 0) {
        char c = keys_lower[scancode];
        if(shift) {
            c = keys_upper[scancode];
        }
        term_putc(c, VGA_COL_WHITE);
    }


    //term_print((char*)scancode);
}

void kb_init() {
    irq_install_handler(1, &kb_handler);
}