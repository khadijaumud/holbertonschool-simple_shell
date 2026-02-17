#include "shell.h"

/**
 * prompt - prints shell prompt
 */
void prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "($) ", 4);
}

/**
 * parse_line - splits input into arguments
 */
char **parse_line(char *line)
{
	char **argv;
	char *token;
	int i = 0;

	argv = malloc(sizeof(char *) * 64);
	if (!argv)
		return (NULL);

	token = strtok(line, " \t\r\n");
	while (token)
	{
		argv[i++] = token;
		token = strtok(NULL, " \t\r\n");
	}
	argv[i] = NULL;
	return (argv);
}

/**
 * execute_cmd - executes command and returns exit status
 */
int execute_cmd(char **argv)
{
	pid_t pid;
	int status, exit_status = 0;
	char *full_path;

	if (argv == NULL || argv[0] == NULL)
		return (0);

	full_path = _which(argv[0]);
	if (full_path == NULL)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", argv[0]);
		return (127);
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(full_path, argv, environ) == -1)
		{
			perror("./hsh");
			free(full_path);
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
	{
		wait(&status);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	free(full_path);
	return (exit_status);
}

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t n_read;
	char **argv;
	int last_status = 0, i;

	while (1)
	{
		prompt();
		n_read = getline(&line, &len, stdin);
		if (n_read == -1)
		{
			free(line);
			exit(last_status);
		}

		argv = parse_line(line);
		if (argv && argv[0])
		{
			if (strcmp(argv[0], "exit") == 0)
			{
				free(argv);
				free(line);
				exit(last_status);
			}
			/* Реализация встроенной команды env */
			if (strcmp(argv[0], "env") == 0)
			{
				for (i = 0; environ[i]; i++)
					printf("%s\n", environ[i]);
				last_status = 0;
			}
			else
				last_status = execute_cmd(argv);
		}
		free(argv);
	}
	return (last_status);
}


