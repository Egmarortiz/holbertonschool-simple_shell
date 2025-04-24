#include "shell.h"

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

