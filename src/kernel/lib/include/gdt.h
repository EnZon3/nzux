#include <stdint.h>
#include "intellisense.h"

struct gdt_entry {
    uint16_t limit;    // Limit
    uint16_t base_low;     // Lower 16 bits of base address
    uint8_t base_middle;   // Next 8 bits of base address
    uint8_t access;        // Access flags
    uint8_t granularity;   // Flags and upper limit bits
    uint8_t base_high;     // Highest 8 bits of base address
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;  // Limit of the GDT table (size - 1)
    uint32_t base;   // Base address of the GDT
} __attribute__((packed));

void gdt_set_gate(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void gdt_init();
void gdt_set();