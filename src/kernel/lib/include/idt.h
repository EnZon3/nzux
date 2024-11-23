#include <stdint.h>
#include "intellisense.h"
#include "util.h"

// Structs for IDT

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t reserved;
    uint8_t flags;
    uint16_t base_up;
} __attribute__((packed));

struct idp {
    uint16_t limit;
    uint32_t addr;
} __attribute__((packed));


void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void exception_handler(struct InterruptRegisters* r);
void irq_handler(struct InterruptRegisters* r);
void irq_load();
void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *r));
void irq_remove_handler(int irq);
void isr_load_exceptions();
void idt_init();


// I HATE THIS SO MUCH

// Exceptions
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// IRQs on Master PIC
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();

// IRQs on Slave PIC
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();