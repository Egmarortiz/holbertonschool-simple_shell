#include "shell.h"

/**
 * split_line - Split a line into tokens
 * @line: The line to split
 *
 * Return: Array of tokens (must be freed by caller)
 */
char **split_line(char *line)
{
	int bufsize = 64, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token, *token_copy;

	if (!tokens)
	{
		perror("malloc");
		return (NULL);
	}

	token = strtok(line, " \t\r\n\a");
	while (token != NULL)
	{
		/* Create a copy of the token */
		token_copy = strdup(token);
		if (!token_copy)
		{
			perror("strdup");
			free_and_null_tokens(tokens, position);
			return (NULL);
		}

		tokens[position] = token_copy;
		position++;

		if (position >= bufsize)
		{
			if (!resize_token_buffer(&tokens, &bufsize, position))
				return (NULL);
		}

		token = strtok(NULL, " \t\r\n\a");
	}
	tokens[position] = NULL;
	return (tokens);
}

/**
 * resize_token_buffer - Resize the tokens buffer
 * @tokens: Pointer to tokens array
 * @bufsize: Pointer to current buffer size
 * @position: Current position in the buffer
 *
 * Return: 1 on success, 0 on failure
 */
int resize_token_buffer(char ***tokens, int *bufsize, int position)
{
	char **new_tokens;
	*bufsize += 64;
	new_tokens = realloc(*tokens, *bufsize * sizeof(char *));
	if (!new_tokens)
	{
		perror("realloc");
		free_and_null_tokens(*tokens, position);
		return (0);
	}
	*tokens = new_tokens;
	return (1);
}

/**
 * free_and_null_tokens - Free tokens and set to NULL
 * @tokens: Tokens array to free
 * @position: Number of tokens to free
 */
void free_and_null_tokens(char **tokens, int position)
{
	while (position > 0)
		free(tokens[--position]);
	free(tokens);
}

/**
 * free_args - Free the arguments array
 * @args: The arguments array to free
 */
void free_args(char **args)
{
	int i;

	if (args != NULL)
	{
		for (i = 0; args[i] != NULL; i++)
			free(args[i]);
		free(args);
	}
}
