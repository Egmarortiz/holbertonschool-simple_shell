#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>

extern char **environ;

void print_prompt(void);
char *read_line(void);
void trim_newline(char *s);
int is_blank(const char *s);
void execute_command(char *cmd, const char *prog_name);

#endif /* SHELL_H */

