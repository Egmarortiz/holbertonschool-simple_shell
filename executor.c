#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* Macro to handle child process error and exit */
#define HANDLE_CHILD_ERROR(p, n) do { \
	perror(n); \
	free(p); \
	exit(errno == ENOENT ? 2 : EXIT_FAILURE); \
} while (0)

/**
 * get_path_env - get the PATH environment variable
 * Return: pointer to PATH value, or NULL if not found
 */
static char *get_path_env(void)
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			return (environ[i] + 5);
		}
	}
	return (NULL);
}

/**
 * search_in_path - search for cmd in PATH directories
 * @cmd: command to search for
 * @path_env: PATH environment variable value
 * Return: malloc'd full path if found (caller must free), or NULL
 */
static char *search_in_path(const char *cmd, char *path_env)
{
	char *path_dup, *dir, *full;
	size_t len;

	path_dup = strdup(path_env);
	if (!path_dup)
		return (NULL);

	for (dir = strtok(path_dup, ":"); dir; dir = strtok(NULL, ":"))
	{
		len = strlen(dir) + 1 + strlen(cmd) + 1;
		full = malloc(len);
		if (!full)
			break;
		snprintf(full, len, "%s/%s", dir, cmd);
		if (access(full, X_OK) == 0)
		{
			free(path_dup);
			return (full);
		}
		free(full);
	}
	free(path_dup);
	return (NULL);
}

/**
 * resolve_path - search for cmd in PATH or use cmd if contains '/'.
 * @cmd: the command name from argv[0].
 * Return: malloc'd full path if found (caller must free), or NULL.
 */
static char *resolve_path(const char *cmd)
{
	char *path_env;

	/* If cmd contains a slash, assume it's a path */
	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}

	/* Get PATH and search in its directories */
	path_env = get_path_env();
	if (!path_env)
		return (NULL);

	return (search_in_path(cmd, path_env));
}

/**
 * resolve_and_execute - resolve path and execute command
 * @args: NULL-terminated argv array
 * @prog_name: argv[0] for error messages
 * @exit_status: pointer to store the command's exit status
 *
 * Return: 1 if command executed, 0 if command not found
 */
static int resolve_and_execute(char **args, const char *prog_name,
							int *exit_status)
{
	pid_t pid;
	char *path;
	int status;

	path = resolve_path(args[0]);
	if (path == NULL)
		return (0);

	pid = fork();
	if (pid == -1)
	{
		perror(prog_name);
		free(path);
		return (1);
	}

	if (pid == 0) /* Child process */
	{
		execve(path, args, environ);
		/* If execve fails, handle the error and exit */
		HANDLE_CHILD_ERROR(path, prog_name);
	}

	/* Parent process */
	free(path);
	waitpid(pid, &status, 0);

	/* Set exit status based on child termination */
	*exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;

	return (1);
}

/**
 * execute_command - resolve cmd, then fork & exec; skip fork if not found
 * @args: NULL-terminated argv array
 * @prog_name: argv[0] for error messages
 * @exit_status: pointer to store the command's exit status
 */
void execute_command(char **args, const char *prog_name, int *exit_status)
{
	if (!resolve_and_execute(args, prog_name, exit_status))
	{
		fprintf(stderr, "%s: 1: %s: not found\n", prog_name, args[0]);
		*exit_status = 127;  /* Standard command not found exit code */
	}
}

