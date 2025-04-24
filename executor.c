#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * execute_command - forks and execve’s the given args
 * @args: NULL-terminated argv array
 * @prog_name: argv[0] for error messages
 */
void execute_command(char **args, const char *prog_name)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror(prog_name);
		return;
	}
	if (pid == 0)
	{
		execve(args[0], args, environ);
		perror(prog_name);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
}

