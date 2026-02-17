#include "shell.h"

/**
 * _getenv - gets the value of an environment variable
 * @name: name of the variable (e.g., "PATH")
 * Return: pointer to the value, or NULL if not found
 */
char *_getenv(const char *name)
{
	int i = 0;
	size_t name_len = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=')
		{
			return (environ[i] + name_len + 1);
		}
		i++;
	}
	return (NULL);
}

/**
 * _which - locates a command in the PATH directories
 * @command: the command name (e.g., "ls")
 * Return: full path string if found, NULL otherwise
 */
char *_which(char *command)
{
	char *path, *path_copy, *token, *full_path;
	struct stat st;

	path = _getenv("PATH"); /* Используем свою функцию вместо getenv */
	if (!path)
		return (NULL);

	if (strchr(command, '/') && stat(command, &st) == 0)
		return (strdup(command));

	path_copy = strdup(path);
	token = strtok(path_copy, ":");
	while (token)
	{
		full_path = malloc(strlen(token) + strlen(command) + 2);
		if (!full_path)
			break;
		sprintf(full_path, "%s/%s", token, command);
		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
