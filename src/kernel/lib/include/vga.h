#ifndef _VGA_H
#define _VGA_H 1

#include <stdint.h>

enum VGA_COLORS {
    VGA_COL_BLACK = 0,
	VGA_COL_BLUE = 1,
	VGA_COL_GREEN = 2,
	VGA_COL_CYAN = 3,
	VGA_COL_RED = 4,
	VGA_COL_MAGENTA = 5,
	VGA_COL_BROWN = 6,
	VGA_COL_LIGHT_GREY = 7,
	VGA_COL_DARK_GREY = 8,
	VGA_COL_LIGHT_BLUE = 9,
	VGA_COL_LIGHT_GREEN = 10,
	VGA_COL_LIGHT_CYAN = 11,
	VGA_COL_LIGHT_RED = 12,
	VGA_COL_LIGHT_MAGENTA = 13,
	VGA_COL_LIGHT_BROWN = 14,
	VGA_COL_WHITE = 15,
};

struct vga_entry_color {
	uint8_t fg : 4;
	uint8_t bg : 3;
	uint8_t blink : 1;
};

struct vga_entry {
	unsigned char character;
	uint8_t color;
};

static inline uint16_t vga_entry(unsigned char character, uint8_t fg, uint8_t bg, uint8_t blink);
/**
 * @brief Initializes the terminal with specified text color, background colors, and blink attribute.
 *
 * This function sets the initial state of the terminal, including the cursor position,
 * default text color, and clears the screen by filling it with spaces.
 *
 * @param fg Text color
 * @param bg Background color
 * @param blink The blink attribute (non-zero value enables blinking).
 */
void term_init(uint8_t fg, uint8_t bg, uint8_t blink);
void term_putc(char c, uint8_t text_col);
void term_nl();
void term_print(const char* str);
void term_rm();

#endif