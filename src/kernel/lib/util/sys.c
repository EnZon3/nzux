#include "kernel.h"
#include "string.h"

extern void* __builtin_return_address(unsigned int level);

void print_stack_trace() {
    void* return_addresses[10];
    return_addresses[0] = __builtin_return_address(0);
    return_addresses[1] = __builtin_return_address(1);
    return_addresses[2] = __builtin_return_address(2);
    return_addresses[3] = __builtin_return_address(3);
    return_addresses[4] = __builtin_return_address(4);
    return_addresses[5] = __builtin_return_address(5);
    return_addresses[6] = __builtin_return_address(6);
    return_addresses[7] = __builtin_return_address(7);
    return_addresses[8] = __builtin_return_address(8);
    return_addresses[9] = __builtin_return_address(9);

    char buffer[32];
    for (int i = 0; i < 10; i++) {
        if (return_addresses[i] == NULL) {
            break;
        }
        term_print("  [");
        itoa((unsigned int)return_addresses[i], buffer, 16);
        term_print(buffer);
        memset(&buffer, 0, 32);
        term_print("]\n");
    }
}

void panic(struct InterruptRegisters *r) {
    term_init(VGA_COL_RED, VGA_COL_BLACK, 0);
    term_print("Exception: ");
    term_print(exception_messages[r->int_no]);
    term_print("\n");
    term_print("Error code: ");
    char buffer[32];
    itoa(r->err_code, buffer, 16);
    term_print(buffer);
    term_print("\n");
    term_print("System halted.\n");

    term_print("Stack trace:\n");
    print_stack_trace();

    __asm__ volatile ("cli");
    __asm__ volatile ("hlt");
}