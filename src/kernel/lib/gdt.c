#include <stdint.h>
#include "gdt.h"
#include "vga.h"

extern void gdt_flush(uint32_t);

struct gdt_entry gdt[5];
struct gdt_ptr gp;

void gdt_set_gate(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;
    gdt[index].limit = limit & 0xFFFF;
    gdt[index].granularity = (limit >> 16) & 0x0F;
    gdt[index].granularity |= gran & 0xF0;
    gdt[index].access = access;
}

void gdt_set() {
    gdt_set_gate(0, 0, 0, 0, 0); // Null segment
    gdt_set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF); // Kernel code segment
    gdt_set_gate(2, 0, 0xFFFFF, 0x92, 0xCF); // Kernel data segment
    gdt_set_gate(3, 0, 0xFFFFF, 0xFA, 0xCF); // User code segment
    gdt_set_gate(4, 0, 0xFFFFF, 0xF2, 0xCF); // User data segment
}

void gdt_init() {
    gp.base = (uint32_t)&gdt;
    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;

    gdt_set();

    gdt_flush((uint32_t)&gp);
    term_print("GDT on");
}