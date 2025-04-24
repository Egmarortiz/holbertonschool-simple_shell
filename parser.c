#include "shell.h"

/**
 * process_token - Process a token and add it to the token array
 * @tokens: Array of tokens
 * @position: Pointer to the current position in the token array
 * @bufsize: Pointer to the current buffer size
 * @token: The token to process
 *
 * Return: 1 on success, 0 on failure
 */
int process_token(char ***tokens, int *position, int *bufsize, char *token)
{
	char *token_copy;

	/* Create a copy of the token */
	token_copy = strdup(token);
	if (!token_copy)
	{
		perror("strdup");
		free_and_null_tokens(*tokens, *position);
		return (0);
	}

	(*tokens)[*position] = token_copy;
	(*position)++;

	/* Check if we need to resize the buffer */
	if (*position >= *bufsize)
	{
		if (!resize_token_buffer(tokens, bufsize, *position))
			return (0);
	}

	return (1);
}

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
	char *token;

	if (!tokens)
	{
		perror("malloc");
		return (NULL);
	}

	/* Simple tokenization on whitespace - no special handling */
	token = strtok(line, " \t\r\n\a");
	while (token != NULL)
	{
		if (!process_token(&tokens, &position, &bufsize, token))
			return (NULL);

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
