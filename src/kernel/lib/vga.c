#include <include/vga.h>

//vars
size_t vga_r;
size_t vga_c;
uint8_t term_col;
uint16_t* vga_buff;

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static inline uint16_t vga_entry(unsigned char character, uint8_t fg, uint8_t bg, uint8_t blink) {
    struct vga_entry ent = {
        .character = character,
        .color = {
            .fg = fg,
            .bg = bg,
            .blink = blink
        }
    };
    return *((uint16_t*)&ent);
};

void term_init() {
    //set vars
    vga_r = 0;
    vga_c = 0;
    enum VGA_COLORS set_col = VGA_COL_WHITE;
    term_col = set_col;
    // clear screen
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buff[i] = vga_entry(' ', VGA_COL_BLACK, term_col, 0);
    };
}

void putc(char c) {
    vga_buff[vga_r * VGA_WIDTH + vga_c] = vga_entry(c, VGA_COL_BLACK, term_col, 0);
}