#include "idt.h"
#include "vga.h"
#include "pic.h"
//#include "kernel.h"

struct idt_entry idt[48];
struct idp idtp;
extern void* isr_stub_table[];
extern void* irq_stub_table[];

void exception_handler(struct InterruptRegisters* r) {
    if(r->int_no < 32) {
        panic(r);
    }
};

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_up = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].reserved = 0;
    idt[num].flags = flags;
}

// Load inerrupts
void isr_load_exceptions() {
    for (unsigned short vec = 0; vec < 32; vec++) {
        idt_set_gate(vec, (uint32_t)isr_stub_table[vec], 0x8, 0x8F);
        term_print("ISR exception gate set\n");
    }
}

void irq_load() {
    for (unsigned short vec = 0; vec < 16; vec++) {
        idt_set_gate(vec + 32, (uint32_t)irq_stub_table[vec], 0x8, 0x8E);
        term_print("IRQ gate set\n");
    }
}

void idt_init() {
    idtp.addr = (uintptr_t)&idt[0];
    idtp.limit = (uint16_t)(sizeof(struct idt_entry) * 48) - 1;
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    PIC_remap(0x20,0x28);

    isr_load_exceptions();
    irq_load();

    __asm__ volatile ("lidt %0" : : "m"(idtp));
    __asm__ volatile ("sti");
}

void *irq_routines[16] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
};

void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *r)) {
    irq_routines[irq] = handler;
}

void irq_remove_handler(int irq) {
    irq_routines[irq] = 0;
}

void irq_handler(struct InterruptRegisters* r) {
    void (*handler)(struct InterruptRegisters *r);

    handler = irq_routines[r->int_no - 32];

    if(handler) {
        handler(r);
    }

    PIC_sendEOI(r->int_no - 32);
}