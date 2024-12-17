#include "pmm_pfa.h"
#include "util.h"
#include <multiboot.h>

#define KERNEL_START 0xC0000000
#define PAGE_SIZE 4096

static uint32_t pf_min;
static uint32_t pf_max;

page_dir_entry init_page_dir[1024];

void invalPage(uint32_t vaddr) {
    asm volatile ("invlpg %0" :: "m"(vaddr));
}

typedef struct {
    uint32_t base_addr;
    uint32_t length;
} usable_memory_t;

static usable_memory_t get_usable_memory(struct multiboot_info* info) {
    usable_memory_t mem_range = {0, 0};
    uint32_t i = info->mmap_addr;
    while (i < info->mmap_addr + info->mmap_length) {
        multiboot_mmap_t *me = (multiboot_mmap_t*)i;

        // Does this entry specify usable RAM?
        if (me->type == 1) {
            mem_range.base_addr = me->addr_low;
            mem_range.length = me->len_low;
            break; // Assuming we want the first usable range; remove this if we need to find all.
        }

        // Move to the next memory map entry
        i += me->size + sizeof(uint32_t);
    }
    return mem_range;
}

void pmm_init(uint32_t mem_low, uint32_t mem_high) {
    pf_min = CEIL_DIV(mem_low, PAGE_SIZE);
    pf_max = mem_high / PAGE_SIZE;

    char buffer[64];
    term_print("mem: Physical memory: ");
    itoa(mem_low, buffer, 16);
    term_print(buffer);
    term_print(" - ");
    memset(buffer, 0, 32);
    itoa(mem_high, buffer, 16);
    term_print(buffer);
    term_print("\n");
}

void init_memory(struct multiboot_info* bootInfo) {
    // get mem start and end
    usable_memory_t mem_range = get_usable_memory(bootInfo);
    uint32_t palloc_start = mem_range.base_addr;

    //uint32_t palloc_start = bootInfo->mmap_addr;
    uint32_t palloc_end = mem_range.base_addr + mem_range.length;

    boot_page_dir[0] = (page_dir_entry){
        .flags = {
            .present = 0,
            .rw = 0,
            .dpl = 0,
            .pwt = 0,
            .pcd = 0,
            .accessed = 0,
            .dirty = 0,
            .ps = 0
        },
        .avl = 0,
        .paddr = 0x0
    };
    invalPage(0);

    boot_page_dir[1023] = (page_dir_entry){
        .flags = {
            .present = 1,
            .rw = 1,
            .dpl = 0,
            .pwt = 0,
            .pcd = 0,
            .accessed = 0,
            .dirty = 0,
            .ps = 0
        },
        .avl = 0,
        .paddr = ((uint32_t) boot_page_dir - KERNEL_START)
    };
    invalPage(0xFFFFF000);

    pmm_init(palloc_start, palloc_end);
}

// paging example
// void initMem() {
//     // Initialize the memory manager
//     // This will be used to manage physical memory
//     // and allocate pages for the page tables
//     // and other kernel structures

//     // Create new test page dir
//     init_page_dir[0] = (page_dir_entry){
//         .flags = {
//             .present = 1,
//             .rw = 1,
//             .dpl = 0,
//             .pwt = 0,
//             .pcd = 0,
//             .accessed = 0,
//             .dirty = 0,
//             .ps = 1
//         },
//         .avl = 0,
//         .paddr = 0
//     };

//     init_page_dir[1] = (page_dir_entry){
//         .flags = {
//             .present = 1,
//             .rw = 1,
//             .dpl = 0,
//             .pwt = 0,
//             .pcd = 0,
//             .accessed = 0,
//             .dirty = 0,
//             .ps = 1
//         },
//         .avl = 0,
//         .paddr = 1 << 22
//     };

//     init_page_dir[2] = (page_dir_entry){
//         .flags = {
//             .present = 1,
//             .rw = 1,
//             .dpl = 0,
//             .pwt = 0,
//             .pcd = 0,
//             .accessed = 0,
//             .dirty = 0,
//             .ps = 1
//         },
//         .avl = 0,
//         .paddr = 2 << 22
//     };

//     init_page_dir[3] = (page_dir_entry){
//         .flags = {
//             .present = 1,
//             .rw = 1,
//             .dpl = 0,
//             .pwt = 0,
//             .pcd = 0,
//             .accessed = 0,
//             .dirty = 0,
//             .ps = 1
//         },
//         .avl = 0,
//         .paddr = 3 << 22
//     };

//     //set cr register pointer to the new page dir
//     term_print("Setting cr3 register to new page dir\n");
//     __asm__ volatile("mov %%cr3, %0" : : "r"(&init_page_dir - 0xC0000000));
// }