#ifndef _CONSOLE.HELPERS_H
#define _CONSOLE.HELPERS_H 1

#define MX_COM_SIZE 400
#define MX_ARG_SIZE 600
#define ITOA_BUF_SIZE 200
#define COM_DELIM " "

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Command {
    char* argv[MX_ARG_SIZE];
    int argc;
} Command;

void read_line(char* buffer, int max_length);
void input_wait();
void scanl(const char* format, char* input_buffer, uint32_t* int_arg, char* str_arg);
int abs(int n);
char* itoa(int value, char* str, int base);
Command parseCommand(char* com);

#ifdef __cplusplus
}
#endif

#endif