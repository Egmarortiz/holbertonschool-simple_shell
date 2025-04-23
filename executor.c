#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * execute_command - forks and execve’s the given cmd.
 * @cmd: full path or relative name of the executable.
 * @prog_name: argv[0] for error messages.
 */
void execute_command(char *cmd, const char *prog_name)
{
	pid_t pid;
	int status;
	char *args[2];

	args[0] = cmd;
	args[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror(prog_name);
		return;
	}
	if (pid == 0)
	{
		execve(cmd, args, environ);
		perror(prog_name);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
}

