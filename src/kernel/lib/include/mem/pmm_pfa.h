#ifndef _PMM_PFA_H
#define _PMM_PFA_H 1

//Includes and other definitions here
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <multiboot.h> // Include the header file that defines struct multiboot_info

#ifdef __cplusplus
extern "C" {
#endif

// note: dirty bit is typically used in 4MB pages but can be used in 4KB pages through the AVL bit in the same position
struct page_dir_entr_flags {
    bool present : 1; // page present when set
    bool rw : 1; // 0: read-only 1: read-write
    bool dpl : 1; // 0: kernel only 1: all rings
    bool pwt : 1; // 0: write-back 1: write-through
    bool pcd : 1; // 0: cached 1: cache disabled
    bool accessed : 1; // page accessed since last refresh
    bool dirty : 1; // 0: clean 1: dirty
    bool ps : 1; // 0: 4KB 1: 4MB
}__attribute__((packed));

typedef struct page_dir_entry {
    struct page_dir_entr_flags flags;
    uint8_t avl: 3; // avail for os to use
    uint32_t paddr; // page table addr
}__attribute__((packed)) page_dir_entry;

extern page_dir_entry boot_page_dir[1024];

//Headers here

void init_memory(struct multiboot_info* bootInfo);

#ifdef __cplusplus
}
#endif

#endif