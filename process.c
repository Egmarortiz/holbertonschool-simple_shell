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
		/* Use write() for consistent error reporting */
		if (args[0] != NULL) {
			write(STDERR_FILENO, args[0], (size_t)strlen(args[0]));
		}
		write(STDERR_FILENO, ": command not found\n", 19);
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
		/* Error forking - use write() for consistent error reporting */
		const char *fork_err = "fork: ";
		write(STDERR_FILENO, fork_err, (size_t)strlen(fork_err));
		
		/* Get error message string */
		char *error_msg = strerror(errno);
		if (error_msg != NULL) {
			write(STDERR_FILENO, error_msg, (size_t)strlen(error_msg));
		}
		write(STDERR_FILENO, "\n", 1);
		
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
 * Description: This function never returns. It either replaces the current
 * process via execve or exits on error.
 */
void handle_child_process(char *command_path, char **args)
{
	char cwd[1024];
	int i;
	char **exec_args = NULL;
	/* Concise debug for command execution */
	const char *debug_header = "DEBUG: Executing: ";
	write(STDERR_FILENO, debug_header, (size_t)strlen(debug_header));
	if (command_path != NULL) {
		write(STDERR_FILENO, command_path, (size_t)strlen(command_path));
	}
	write(STDERR_FILENO, "\n", 1);
	
	/* Count arguments */
	for (i = 0; args[i] != NULL; i++)
		; /* Just count the arguments */
	
	/* Check if command path is different from args[0] (direct path case) */
	if (strcmp(command_path, args[0]) != 0)
	{
		const char *debug_diff = "DEBUG: Command path differs from args[0], creating new args array\n";
		write(STDERR_FILENO, debug_diff, (size_t)strlen(debug_diff));
		
		/* Create a new exec_args array with command_path as first arg and preserve all original arguments */
		exec_args = malloc((i + 2) * sizeof(char *)); /* +1 for command_path and +1 for NULL */
		if (!exec_args)
		{
			const char *err_msg = "ERROR: malloc: Cannot allocate memory\n";
			write(STDERR_FILENO, err_msg, (size_t)strlen(err_msg));
			exit(EXIT_FAILURE);
		}
		
		/* Set command_path as first argument */
		exec_args[0] = command_path;
		
		/* Copy all original arguments to positions starting at index 1 */
		for (i = 0; args[i] != NULL; i++)
		{
			exec_args[i+1] = args[i];
		}
		exec_args[i+1] = NULL; /* Ensure NULL termination */
	}
	else
	{
		/* Use the original args array */
		exec_args = args;
	}

	/* Get current working directory for debugging */
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		const char *dir_prefix = "DEBUG: Working dir: ";
		write(STDERR_FILENO, dir_prefix, (size_t)strlen(dir_prefix));
		write(STDERR_FILENO, cwd, (size_t)strlen(cwd));
		write(STDERR_FILENO, "\n", 1);
	}
	
	/* Clear any pending error conditions */
	errno = 0;

	/* Final notification before execution */
	const char *exec_msg = "DEBUG: Executing now\n";
	write(STDERR_FILENO, exec_msg, (size_t)strlen(exec_msg));

	/* Execute the command with the current environment */
	/* If execve succeeds, the following code is never reached */
	if (execve(command_path, exec_args, environ) == -1)
	{
		/* Handle execve failure - this function exits and doesn't return */
		handle_execve_error(command_path, args, exec_args);
	}

	/* This code is never reached (execve replaces process on success, 
	   and handle_execve_error exits on failure) */
}

/**
 * handle_execve_error - Handle error when execve fails
 * @command_path: Path to the command that failed
 * @args: Original arguments array
 * @exec_args: Execution arguments array (may be different from args)
 */
void handle_execve_error(char *command_path, char **args, char **exec_args)
{
	/* Debug output for execve error */
	const char *err_prefix = "DEBUG: execve failed with error: ";
	write(STDERR_FILENO, err_prefix, (size_t)strlen(err_prefix));
	
	char error_num[16];
	snprintf(error_num, sizeof(error_num), "%d", errno);
	write(STDERR_FILENO, error_num, (size_t)strlen(error_num));
	write(STDERR_FILENO, "\n", 1);
	
	/* Write error message directly for consistency */
	if (args[0] != NULL) {
		write(STDERR_FILENO, args[0], (size_t)strlen(args[0]));
	}
	write(STDERR_FILENO, ": ", 2);
	
	/* Get error message string */
	char *error_msg = strerror(errno);
	if (error_msg != NULL) {
		write(STDERR_FILENO, error_msg, (size_t)strlen(error_msg));
	}
	write(STDERR_FILENO, "\n", 1);
	
	/* Clean up memory if we created a new args array */
	if (exec_args != args)
	{
		const char *free_msg = "DEBUG: Freeing custom exec_args array\n";
		write(STDERR_FILENO, free_msg, (size_t)strlen(free_msg));
		free(exec_args);
	}
	
	exit(EXIT_FAILURE);
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

