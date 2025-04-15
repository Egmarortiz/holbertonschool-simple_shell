#include "shell.h"

/**
 * is_valid_number - Check if a string is a valid number
 * @str: String to check
 *
 * Return: 1 if valid number, 0 otherwise
 */
int is_valid_number(char *str)
{
	int i = 0;

	if (str == NULL || *str == '\0')
		return (0);

	/* Allow optional negative sign */
	if (str[0] == '-')
		i = 1;

	/* Check that all remaining characters are digits */
	for (; str[i] != '\0'; i++)
	{
		if (!isdigit(str[i]))
			return (0);
	}

	return (1);
}

/**
 * shell_exit - Built-in command to exit the shell
 * @args: Array of arguments
 * @state: Pointer to the shell state
 *
 * Return: 0 to exit the shell, 2 for error case to continue shell
 */
int shell_exit(char **args, shell_state_t *state)
{
	int status;

	if (args[1] != NULL)
	{
		if (is_valid_number(args[1]))
		{
			status = atoi(args[1]);
			if (status < 0)
			{
				fprintf(stderr, "exit: Illegal number: %s\n", args[1]);
				state->exit_status = 2;
				return (2); /* Error code - continue shell */
			}
			state->exit_status = status & 255; /* Limit to 8 bits */
		}
		else
		{
			fprintf(stderr, "exit: Illegal number: %s\n", args[1]);
			state->exit_status = 2;
			return (2); /* Error code - continue shell */
		}
	}

	return (0); /* Exit shell with last_exit_status */
}

/**
 * check_for_builtin - Check if command is a built-in and execute it
 * @args: Array of arguments
 * @state: Pointer to the shell state
 *
 * Return: 0 if built-in was executed and shell should exit,
 *         1 if not a built-in command (continue to external command),
 *         2 if built-in was executed with error and shell should continue
 */
int check_for_builtin(char **args, shell_state_t *state)
{
	int builtin_exit_status;

	if (strcmp(args[0], "exit") == 0)
	{
		builtin_exit_status = shell_exit(args, state);
		return (builtin_exit_status); /* Return 0 for exit, 2 for error */
	}

	return (1); /* Not a builtin */
}

