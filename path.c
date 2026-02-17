#include "shell.h"


/**
 * _which - locates a command in the PATH directories
 * @command: the command name (e.g., "ls")
 *
 * Return: full path to the command if found, NULL otherwise
 */
char *_which(char *command)
{
    char *path = getenv("PATH");
    char *path_copy, *token, *full_path;
    struct stat st;

    if (!path)
        return (NULL);

    if (strchr(command, '/') && stat(command, &st) == 0)
        return (strdup(command));

    path_copy = strdup(path);
    token = strtok(path_copy, ":");

    while (token)
    {
        full_path = malloc(strlen(token) + strlen(command) + 2);
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

