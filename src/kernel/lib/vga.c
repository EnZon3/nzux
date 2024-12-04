#include "include/vga.h"
#include "util.h"
#include <stdint.h>
#include <stddef.h>

//vars
size_t vga_r;
size_t vga_c;
uint16_t* vga_buff;
uint8_t term_bg;
uint8_t term_fg;

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static inline uint16_t vga_entry(unsigned char character, uint8_t fg, uint8_t bg, uint8_t blink) {
    uint8_t color = fg | bg << 4 | blink << 7;
    return (uint16_t) character | (uint16_t) color << 8;
}

static void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void term_init(uint8_t fg, uint8_t bg, uint8_t blink) {
    //set vars
    vga_r = 0;
    vga_c = 0;
    term_bg = bg;
    term_fg = fg;
    vga_buff = (uint16_t*) 0xB8000;

    //disable bios cursor
    disable_cursor();
    
    // clear screen
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buff[index] = vga_entry(' ', fg, bg, blink);
        }
    }
}

void term_rm() {
    vga_buff[vga_r * VGA_WIDTH + vga_c - 1] = vga_entry(' ', term_fg, term_bg, 0);
    if (--vga_c == VGA_WIDTH) {
        vga_c = 0;
        if (--vga_r == VGA_HEIGHT) {
            vga_r = 0;
        }
    }
}

void term_putc(char c, uint8_t text_col) {
    switch(c) {
        case '\n':
            vga_c = 0;
            if (++vga_r == VGA_HEIGHT) {
                vga_r = 0;
            }
        break;
        case '\b':
            vga_buff[vga_r * VGA_WIDTH + vga_c - 1] = vga_entry(' ', text_col, term_bg, 0);
            if (--vga_c == VGA_WIDTH) {
                vga_c = 0;
                if (--vga_r == VGA_HEIGHT) {
                    vga_r = 0;
                }
            }
        break;
        default:
            vga_buff[vga_r * VGA_WIDTH + vga_c] = vga_entry(c, text_col, term_bg, 0);
            //inc cursor
            if (++vga_c == VGA_WIDTH) {
                vga_c = 0;
                if (++vga_r == VGA_HEIGHT) {
                    vga_r = 0;
                }
            }
        break;
    }
}

void term_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {

        switch(str[i]) {
            case '\n':
                vga_c = 0;
                if (++vga_r == VGA_HEIGHT) {
                    vga_r = 0;
                }
            break;
            case '\b':
                // do nothing
            break;
            default:
                term_putc(str[i], term_fg);
            break;
        }
    }
}