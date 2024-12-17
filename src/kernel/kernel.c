#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "console.h"
#include "pmm_pfa.h"
#include "multiboot.h"
//#include "kb.h"

char* buf = NULL;

char splash[] = 
" _ __   ____ _   _ __  __\n"
"| \'_ \\ |_  /| | | |\\ \\/ /\n"
"| | | | / / | |_| | >  < \n"
"|_| |_|/___| \\__,_|/_/\\_\\\n";

void displaySplash() {
    term_init(VGA_COL_WHITE, VGA_COL_BLACK, 0);
    term_print(splash);
    term_print("Welcome to nzux!\n");
    term_print("sys: starting terminal\n");
};

extern uint32_t _kernel_end;

int main(struct multiboot_info* bootInfo);

int main(struct multiboot_info* bootInfo) {
    term_init(VGA_COL_WHITE, VGA_COL_BLACK, 0);
    // init
    gdt_init();
    idt_init();
    kb_init(&buf);

    // uint32_t mod1 = *(uint32_t*)(bootInfo->mods_addr + 4);
    // uint32_t physicalAllocStart = (mod1 + 0xFFF) & ~0xFFF;

    // kernel splash
    displaySplash();

    //push multiboot info to pointer
    struct multiboot_info mBootInfo = *bootInfo;

    // //dump multiboot info
    // term_print("sys: multiboot info\n");
    // for (int i = 0; i < 32; i++) {
    //     char* str = "sys: ";
    //     term_print(str);
    //     term_print(itoa(i, NULL, 10));
    //     term_print(": ");
    //     term_print(itoa(((uint32_t*)&mBootInfo)[i], NULL, 16));
    //     term_print("\n");
    // }

    char buffer[32];
    term_print("sys: mmap addr: ");
    term_print(itoa(mBootInfo.mmap_addr, buffer, 16));
    term_print("\n");
    memset(buffer, 0, 32);
    term_print("sys: mmap length: ");
    term_print(itoa(mBootInfo.mmap_length, buffer, 10));
    term_print("\n");

    //init_memory(&mBootInfo);

    // terminal start
    startTerm(buf, NULL);

    term_print("tty: terminated\n");
    term_print("sys: halt");
    __asm__ volatile("hlt; cli");

    while (1);
}