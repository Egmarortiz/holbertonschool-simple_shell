#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>

extern char **environ;

void print_prompt(void);
char *read_line(void);
void trim_newline(char *s);
int is_blank(const char *s);
char **split_line(char *line);
void free_args(char **args);
void execute_command(char **args, const char *prog_name, int *exit_status);
void print_environment(void);
int handle_command(char **args, char *line, const char *prog_name, int *should_exit, int current_status);

#endif /* SHELL_H */

