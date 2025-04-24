#include "shell.h"

/**
 * launch_process - Launch a process with the given arguments
 * @args: Array of arguments
 * @state: Pointer to shell state structure
 *
 * Return: 1 to continue the shell loop
 */
int launch_process(char **args, shell_state_t *state)
{
	pid_t pid;
	char *command_path;

	command_path = find_command_path(args[0]);
	if (command_path == NULL)
	{
		fprintf(stderr, "%s: command not found\n", args[0]);
		state->exit_status = 127; /* Command not found exit status */
		return (1);
	}

	pid = fork_process();
	if (pid == 0)
	{
		/* Child process */
		handle_child_process(command_path, args);
		exit(EXIT_FAILURE); /* Should never reach here */
	}
	else if (pid < 0)
	{
		/* Error forking */
		perror("fork");
		state->exit_status = 1;
	}
	else
	{
		/* Parent process */
		wait_for_process(pid, state);
	}

	free(command_path);
	return (1);
}

/**
 * handle_child_process - Handle execution in child process
 * @command_path: Full path to the command
 * @args: Command arguments
 *
 * Return: Only returns if execve fails
 */
int handle_child_process(char *command_path, char **args)
{
	if (execve(command_path, args, environ) == -1)
	{
		perror(args[0]);
		exit(EXIT_FAILURE);
	}
	return (EXIT_FAILURE); /* Should never reach here */
}

/**
 * fork_process - Fork a new process
 *
 * Return: PID of the new process, -1 on failure
 */
pid_t fork_process(void)
{
	return (fork());
}

/**
 * wait_for_process - Wait for a process to finish
 * @pid: Process ID to wait for
 * @state: Pointer to shell state structure
 *
 * Return: 1 to continue shell loop
 */
int wait_for_process(pid_t pid, shell_state_t *state)
{
	pid_t wpid;
	int status;

	do {
		wpid = waitpid(pid, &status, WUNTRACED);
	} while (!WIFEXITED(status) && !WIFSIGNALED(status) && wpid != -1);

	if (WIFEXITED(status))
		state->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		state->exit_status = 128 + WTERMSIG(status);

	return (1);
}

