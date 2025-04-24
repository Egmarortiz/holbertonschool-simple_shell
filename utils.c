#include "shell.h"

/**
 * validate_command_path - Check if a command path is valid
 * @command_path: Path to check
 *
 * Return: 1 if valid, 0 if not
 */
int validate_command_path(char *command_path)
{
	struct stat buffer;

	if (command_path == NULL)
		return (0);

	if (stat(command_path, &buffer) == 0)
		return (1);

	return (0);
}

/**
 * find_command_path - Find the full path of a command
 * @command: Command to find
 *
 * Return: Full path to the command if found, NULL otherwise
 */
char *find_command_path(char *command)
{
	char *path_env;
	struct stat buffer;

	/* If command contains a slash, check if it's a valid path directly */
	if (strchr(command, '/') != NULL)
	{
		if (stat(command, &buffer) == 0)
			return (strdup(command));
		return (NULL);
	}

	path_env = getenv("PATH");
	if (path_env == NULL)
		return (NULL);

	return (search_in_path(command, path_env));
}

/**
 * search_in_path - Search for command in PATH directories
 * @command: Command to find
 * @path_env: PATH environment variable
 *
 * Return: Full path if found, NULL otherwise
 */
char *search_in_path(char *command, char *path_env)
{
	char *path_copy, *path_token, *file_path;
	int command_len, dir_len;

	path_copy = strdup(path_env);
	if (path_copy == NULL)
		return (NULL);

	command_len = strlen(command);
	path_token = strtok(path_copy, ":");

	while (path_token != NULL)
	{
		dir_len = strlen(path_token);
		file_path = malloc(dir_len + command_len + 2);
		if (file_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}

		strcpy(file_path, path_token);
		strcat(file_path, "/");
		strcat(file_path, command);

		if (validate_command_path(file_path))
		{
			free(path_copy);
			return (file_path);
		}

		free(file_path);
		path_token = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

