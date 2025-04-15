#ifndef SHELL_H
#define SHELL_H

/* Standard library headers */
#include <stdio.h>     /* For printf, fprintf, perror */
#include <stdlib.h>    /* For malloc, free, exit, atoi */
#include <unistd.h>    /* For fork, execve, access, chdir, getcwd, etc. */
#include <string.h>    /* For strcmp, strcpy, strcat, strdup, strlen */
#include <sys/types.h> /* For pid_t and other type definitions */
#include <sys/wait.h>  /* For wait, waitpid */
#include <sys/stat.h>  /* For stat */
#include <fcntl.h>     /* For open */
#include <errno.h>     /* For errno */
#include <signal.h>    /* For signal handling */
#include <dirent.h>    /* For opendir, readdir, closedir */
#include <ctype.h>     /* For isdigit */

/* External environment variable */
extern char **environ;

/**
 * struct shell_state - Structure to hold shell state
 * @exit_status: Exit status of the last command
 *
 * Description: Contains state information for the shell
 */
typedef struct shell_state
{
	int exit_status;
} shell_state_t;

/* Initialize shell state in main.c */
shell_state_t init_shell_state(void);

/* Main shell functions (main.c) */
int shell_loop(shell_state_t *state);
int process_command(char *line, shell_state_t *state);

/* Interactive mode functions (shell_interactive.c) */
void display_prompt(void);
int is_interactive(void);
void handle_signal(int sig);
char *read_line(void);

/* Command parsing functions (shell_parser.c) */
char **split_line(char *line);
int resize_token_buffer(char ***tokens, int *bufsize, int position);
void free_and_null_tokens(char **tokens, int position);
void free_args(char **args);

/* Command execution functions (shell_execute.c) */
int execute_command(char **args, shell_state_t *state);
int launch_external_process(char **args, shell_state_t *state);

/* Path resolution functions (shell_utils.c) */
char *find_command_path(char *command);
char *search_in_path(char *command, char *path_env);
int validate_command_path(char *command_path);

/* Process management functions (shell_process.c) */
int launch_process(char **args, shell_state_t *state);
pid_t fork_process(void);
int handle_child_process(char *command_path, char **args);
int wait_for_process(pid_t pid, shell_state_t *state);

/* Built-in commands (shell_builtins.c) */
int shell_exit(char **args, shell_state_t *state);
int check_for_builtin(char **args, shell_state_t *state);
int is_valid_number(char *str);

#endif /* SHELL_H */
