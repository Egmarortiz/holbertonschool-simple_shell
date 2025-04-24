#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * resolve_path - search for cmd in PATH or use cmd if contains '/'.  
 * @cmd: the command name from argv[0].  
 * Return: malloc’d full path if found (caller must free), or NULL.
 */
static char *resolve_path(const char *cmd)
{
	char *path_env, *path_dup, *dir, *full;
	size_t len;

	/* If cmd contains a slash, assume it's a path */
	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}

	/* Get PATH and iterate its directories */
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
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
 * execute_command - resolve cmd, then fork & exec; skip fork if not found.
 * @args: NULL-terminated argv array.
 * @prog_name: argv[0] for error messages.
 */
void execute_command(char **args, const char *prog_name)
{
	pid_t pid;
	int status;
	char *path;

	/* Resolve the executable’s path first */
	path = resolve_path(args[0]);
	if (path == NULL)
	{
		fprintf(stderr, "%s: %s: not found\n", prog_name, args[0]);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror(prog_name);
		free(path);
		return;
	}
	if (pid == 0)
	{
		execve(path, args, environ);
		perror(prog_name);
		free(path);
		exit(EXIT_FAILURE);
	}
	/* Parent frees path and waits */
	free(path);
	waitpid(pid, &status, 0);
}

