#include <stdbool.h>
#include <stdint.h>
#include "string.h"
#include "console_helpers.h"

extern volatile bool input_ready;
extern char text[2000];

void read_line(char* buffer, int max_length) {
    int i = 0;
    while (i < max_length - 1) {
        // Wait until input is ready
        while (!input_ready);

        // Copy the input from the text buffer
        buffer[i] = text[i];
        if (text[i] == '\n') {
            buffer[i + 1] = '\0';
            clear_txt();
            input_ready = false;
            return;
        }
        i++;
    }
    buffer[i] = '\0';
}

void input_wait() {
    while (!input_ready);
    input_ready = false;
    return;
}

void scanl(const char* format, char* input_buffer, uint32_t* int_arg, char* str_arg) {
    input_wait();
    const char* p = format;
    char* buf_p = input_buffer;

    while (*p != '\0') {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'd':
                    *int_arg = 0;
                    while (*buf_p >= '0' && *buf_p <= '9') {
                        *int_arg = *int_arg * 10 + (*buf_p - '0');
                        buf_p++;
                    }
                    break;
                case 's':
                    while (*buf_p != '\n' && *buf_p != '\0') {
                        *str_arg = *buf_p;
                        str_arg++;
                        buf_p++;
                    }
                    *str_arg = '\0';
                    break;
                default:
                    break;
            }
        }
        p++;
    }
    clear_txt();
}

int abs(int n) {
    return (n < 0) ? -n : n;
}

char* itoa(int value, char* str, int base) {
    char* rc;
    char* ptr;
    char* low;
    
    // Check for supported base.
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    rc = ptr = str;

    // Handle zero explicitly, otherwise empty string is printed for zero.
    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }

    // Set '-' for negative decimals.
    bool isNegative = (value < 0 && base == 10);
    if (isNegative) {
        *ptr++ = '-';
    }

    // Remember where the numbers start.
    low = ptr;

    // The actual conversion.
    while (value) {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[abs(value % base)];
        value /= base;
    }

    // Terminating the string.
    *ptr = '\0';

    // Invert the numbers.
    for (--ptr; low < ptr; ++low, --ptr) {
        char tmp = *low;
        *low = *ptr;
        *ptr = tmp;
    }

    return rc;
}

Command parseCommand(char* com) {
    Command cmd;
    cmd.argc = 0;

    char* token = strtok(com, " ");
    while (token != NULL && cmd.argc < MX_ARG_SIZE) {
        cmd.argv[cmd.argc] = token;
        cmd.argc++;
        token = strtok(NULL, " ");
    }

    return cmd;
}
