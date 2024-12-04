#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "console.h"
//#include "kb.h"

char* buf = NULL;

int main() {
    gdt_init();
    idt_init();
    kb_init(&buf);

    term_init(VGA_COL_WHITE, VGA_COL_BLACK, 0);
    term_print("Welcome to nzux!\n");
    term_print("sys: starting terminal\n");
    
    startTerm(buf, NULL);

    term_print("tty: terminated\n");
    term_print("sys: halt");
    __asm__ volatile("hlt; cli");

    while (1);
}