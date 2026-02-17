#include "shell.h"

/**
 * _getenv - gets the value of an environment variable
 * @name: name of the variable
 * Return: pointer to the value, or NULL if not found
 */
char *_getenv(const char *name)
{
	int i = 0;
	size_t name_len = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=')
			return (environ[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

/**
 * _which - locates a command in the PATH directories
 * @command: the command name
 * Return: full path string if found, NULL otherwise
 */
char *_which(char *command)
{
	char *path, *path_copy, *token, *full_path;
	struct stat st;

	if (!command)
		return (NULL);

	/* ПЕРВАЯ ПРОВЕРКА: Если путь уже полный или относительный */
	if (strchr(command, '/'))
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		return (NULL);
	}

	path = _getenv("PATH");
	if (!path || *path == '\0')
		return (NULL);

	path_copy = strdup(path);
	token = strtok(path_copy, ":");
	while (token)
	{
		full_path = malloc(strlen(token) + strlen(command) + 2);
		if (full_path)
		{
			sprintf(full_path, "%s/%s", token, command);
			if (stat(full_path, &st) == 0)
			{
				free(path_copy);
				return (full_path);
			}
			free(full_path);
		}
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);	
}





