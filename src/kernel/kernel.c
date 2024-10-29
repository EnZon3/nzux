#include "lib/include/vga.h"

int main() {
    term_init(VGA_COL_WHITE, VGA_COL_BLACK, 0);
    term_print("Hello, World!\n");
    term_print("Goodbye, World!\n");
    while (1);
}