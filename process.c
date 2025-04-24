#include "shell.h"

/**
 * launch_process - Launch a process
 * @args: Array of command arguments
 * @state: Pointer to shell state
 *
 * Return: 1 on success, 0 on failure
 */
int launch_process(char **args, shell_state_t *state)
{
	char *command_path;
	pid_t pid;
	const char *fork_err;
	char *error_msg;

	command_path = find_command_path(args[0]);
	if (command_path == NULL)
	{
		write_command_error(args[0], "not found");
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
		fork_err = "fork: ";
		write(STDERR_FILENO, fork_err, strlen(fork_err));
		error_msg = strerror(errno);
		if (error_msg != NULL)
			write(STDERR_FILENO, error_msg, strlen(error_msg));
		write(STDERR_FILENO, "\n", 1);
		state->exit_status = 1;
	}
	else
		wait_for_process(pid, state);

	free(command_path);
	return (1);
}

/**
 * prepare_exec_args - Prepare arguments for execve
 * @command_path: Full path to the command
 * @args: Array of command arguments
 *
 * Return: Newly allocated array of arguments for execve, or NULL on error
 */
char **prepare_exec_args(const char *command_path, char * const *args)
{
	char **exec_args;
	int arg_count;
	int i;

	/* Count arguments */
	for (arg_count = 0; args[arg_count] != NULL; arg_count++)
		;

	/* Allocate memory for new args array (command_path + args + NULL) */
	exec_args = malloc(sizeof(char *) * (arg_count + 2));
	if (exec_args == NULL)
	{
		write(STDERR_FILENO, "Memory allocation error\n", 24);
		return (NULL);
	}

	/* Set up argument array */
	exec_args[0] = (char *)command_path;  /* Safe cast: execve won't modify */
	for (i = 0; i < arg_count; i++)
		exec_args[i + 1] = (char *)args[i];
	exec_args[arg_count + 1] = NULL;

	return (exec_args);
}

/**
 * check_command_permissions - Check if command is executable
 * @command_path: Full path to the command
 * @exec_args: Arguments array for execve (to free if error)
 *
 * Return: 0 if executable, otherwise appropriate error code
 */
int check_command_permissions(const char *command_path, char **exec_args)
{
	const char *perm_err;

	if (access(command_path, X_OK) != 0)
	{
		if (errno == EACCES)
		{
			perm_err = "Permission denied: ";
			write(STDERR_FILENO, perm_err, strlen(perm_err));
			write(STDERR_FILENO, command_path, strlen(command_path));
			write(STDERR_FILENO, "\n", 1);
			free(exec_args);
			return (126);
		}
		free(exec_args);
		return (127);
	}
	return (0);
}

/**
 * handle_child_process - Handle the child process execution
 * @command_path: Full path to the command
 * @args: Array of command arguments
 *
 * Return: Should never return if successful
 */
int handle_child_process(const char *command_path, char * const *args)
{
	char **exec_args;
	int perm_check;
	const char *exec_msg;

	exec_args = prepare_exec_args(command_path, args);
	if (exec_args == NULL)
		return (1);

	/* Reset signal handling */
	signal(SIGINT, SIG_DFL);

	/* Check permissions */
	perm_check = check_command_permissions(command_path, exec_args);
	if (perm_check != 0)
		return (perm_check);

	/* Execute command */
	exec_msg = "DEBUG: Executing now\n";
	write(STDERR_FILENO, exec_msg, strlen(exec_msg));

	if (execve(command_path, exec_args, environ) == -1)
		handle_execve_error(command_path, args, exec_args);

	/* Should never reach here */
	free(exec_args);
	return (1);
}

/**
 * handle_execve_error - Handle execve failures
 * @command_path: Full path to the command
 * @args: Original command arguments
 * @exec_args: Arguments array for execve
 */
void handle_execve_error(
	const char *command_path,
	char * const *args,
	char **exec_args)
{
	const char *err_prefix;
	const char *err_msg;
	char error_num[16];

	(void)args;  /* Suppress unused parameter warning */

	/* Set up all variables at start of function */
	err_prefix = "DEBUG: execve failed with error: ";
	err_msg = strerror(errno);

	/* Write error messages */
	write(STDERR_FILENO, err_prefix, strlen(err_prefix));
	snprintf(error_num, sizeof(error_num), "%d", errno);
	write(STDERR_FILENO, error_num, strlen(error_num));
	write(STDERR_FILENO, "\n", 1);

	/* Write command-specific error */
	write(STDERR_FILENO, command_path, strlen(command_path));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, err_msg, strlen(err_msg));
	write(STDERR_FILENO, "\n", 1);

	/* Clean up and exit */
	free(exec_args);

	/* Exit with appropriate error code */
	if (errno == ENOENT)
		exit(127); /* Command not found */
	else if (errno == EACCES)
		exit(126); /* Permission denied */
	else
		exit(EXIT_FAILURE); /* Other errors */
}
