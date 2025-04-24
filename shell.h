#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>

/* Shell state structure */
typedef struct shell_state {
  int exit_status;
} shell_state_t;

/* Environment variable */
extern char **environ;

/* Function prototypes */

/* Main functions */
shell_state_t init_shell_state(void);
int shell_loop(shell_state_t *state);
void handle_signal(int signo);

/* Command processing functions */
int process_command(char *line, shell_state_t *state);
int handle_empty_command(char **args);
int execute_parsed_command(char **args, shell_state_t *state);

/* Shell mode functions */
int handle_pipe_mode(shell_state_t *state);
int handle_interactive_mode(shell_state_t *state);

/* Input/output functions */
char *read_line(void);
char **split_line(char *line);
void display_prompt(void);
int is_interactive(void);
void free_args(char **args);
void write_command_error(const char *command, const char *error);

/* Command execution functions */
int execute_command(char **args, shell_state_t *state);
char *find_command_path(const char *command);
int validate_command_path(const char *command_path);
char *search_in_path(const char *command, const char *path_env);

/* Process management functions */
int launch_process(char **args, shell_state_t *state);
pid_t fork_process(void);
int handle_child_process(const char *command_path, char * const *args);
void handle_execve_error(const char *command_path, char * const *args, char **exec_args);
int wait_for_process(pid_t pid, shell_state_t *state);

/* Built-in commands */
int shell_exit(char **args, shell_state_t *state);
int check_for_builtin(char **args, shell_state_t *state);
int is_valid_number(char *str);

/* Parser functions */
void free_and_null_tokens(char **tokens, int position);
int resize_token_buffer(char ***tokens, int *bufsize, int position);

#endif /* SHELL_H */
