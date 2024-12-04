#include <stdint.h>
#include <stdbool.h>
#include "vga.h"
#include "string.h"
#include "kb.h"
#include "console_helpers.h"

void runCommand(Command cmd) {
    term_putc('\n', VGA_COL_WHITE);
    if (strcmp(cmd.argv[0], "help") == 0) {
        term_print("Help command");
    } else {
        term_print(cmd.argv[0]);
        term_print(": command not found");
    }
}

void startTerm(char* stdin/*, char* argv*/) {
    // rudimentary terminal app
    term_init(VGA_COL_WHITE, VGA_COL_BLACK, 0);

    char str[MX_COM_SIZE];

    // while(1) {
    //     term_print("Enter a number: ");
    //     //read_line(stdin, 2000); // Read input into stdin buffer
    //     scanl("%d", stdin, &num, NULL);

    //     term_print("\nEnter a string: ");
    //     //read_line(stdin, 2000); // Read input into stdin buffer
    //     scanl("%s", stdin, NULL, str);

    //     term_print("\nYou entered: ");
    //     term_print(num);
    //     term_print(" and ");
    //     term_print(str);
    //     term_print("\n");
    //     return;
    // }

    while(1) {
        char itoa_buf[ITOA_BUF_SIZE]; // itoa buf
        term_print("kernel@nzux:~$ ");
        scanl("%s", stdin, NULL, str);
        Command cmd = parseCommand(str);
        runCommand(cmd);
        term_putc('\n', VGA_COL_WHITE);
    }
}