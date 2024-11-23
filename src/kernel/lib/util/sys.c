#include "kernel.h"

void panic(struct InterruptRegisters *r) {
    term_init(VGA_COL_RED, VGA_COL_BLACK, 0);
    term_print("Exception: ");
    term_print(exception_messages[r->int_no]);
    term_print("\n");
    term_print("System halted.");

    __asm__ volatile ("cli");
    __asm__ volatile ("hlt");
}