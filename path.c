#include "shell.h"

/**
 * _getenv - gets an environment variable without using standard getenv
 */
char *_getenv(const char *name)
{
	int i = 0;
	size_t len = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (environ[i] + len + 1);
		i++;
	}
	return (NULL);
}

/**
 * _which - finds a command in PATH
 */
char *_which(char *command)
{
	char *path, *path_cp, *token, *full_path;
	struct stat st;

	if (strchr(command, '/'))
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		return (NULL);
	}

	path = _getenv("PATH");
	if (!path || *path == '\0')
		return (NULL);

	path_cp = strdup(path);
	token = strtok(path_cp, ":");
	while (token)
	{
		full_path = malloc(strlen(token) + strlen(command) + 2);
		sprintf(full_path, "%s/%s", token, command);
		if (stat(full_path, &st) == 0)
		{
			free(path_cp);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path_cp);
	return (NULL);
}

