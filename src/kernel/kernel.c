#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "kb.h"

int main() {
    gdt_init();
    idt_init();
    kb_init();

    term_init(VGA_COL_WHITE, VGA_COL_BLACK, 0);
    term_print("Hello, World!\n");
    term_print("Goodbye, World!\n");
    while (1);
}