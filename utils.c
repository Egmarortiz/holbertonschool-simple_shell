#include "shell.h"
#include <sys/stat.h>

/**
 * validate_command_path - Check if a path points to an executable
 * @command_path: Path to check
 *
 * Return: 1 if valid, 0 otherwise
 */
int validate_command_path(const char *command_path)
{
	struct stat st;

	if (stat(command_path, &st) == 0)
	{
		if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
			return (1);
	}
	return (0);
}

/**
 * find_command_path - Find the full path of a command
 * @command: Command to find
 *
 * Return: Full path of command if found, NULL otherwise
 */
char *find_command_path(const char *command)
{
	const char *path_env;

	if (command == NULL || *command == '\0')
		return (NULL);

	if (strchr(command, '/') != NULL)
	{
		if (validate_command_path(command))
			return (strdup(command));
		return (NULL);
	}

	path_env = getenv("PATH");
	if (path_env == NULL)
		return (NULL);

	return (search_in_path(command, path_env));
}

/**
 * search_in_path - Search for a command in PATH
 * @command: Command to find
 * @path_env: PATH environment variable value
 *
 * Return: Full path if found, NULL otherwise
 */
char *search_in_path(const char *command, const char *path_env)
{
	char *path_copy;
	char *dir;
	char *full_path;
	size_t path_len;

	path_copy = strdup(path_env);
	if (path_copy == NULL)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		path_len = strlen(dir) + strlen(command) + 2;
		full_path = malloc(path_len);
		if (full_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}

		sprintf(full_path, "%s/%s", dir, command);
		if (validate_command_path(full_path))
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
