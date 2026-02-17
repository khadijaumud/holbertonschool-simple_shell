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
 * @line: user input
 *
 * Return: array of arguments
 */
char **parse_line(char *line)
{
	char **argv;
	char *token;
	int i = 0;

	argv = malloc(sizeof(char *) * 64);
	if (argv == NULL)
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
 * execute_cmd - executes command
 * @argv: arguments array
 */
void execute_cmd(char **argv)
{
	pid_t pid;
	int status;

	if (argv[0] == NULL)
		return;

	pid = fork();

	if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
			perror("./hsh");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		wait(&status);
	}
	else
	{
		perror("fork");
	}
}

/**
 * free_args - frees argument array
 * @argv: argument array
 */
void free_args(char **argv)
{
	free(argv);
}

/**
 * main - simple shell entry point
 *
 * Return: Always 0
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char **argv;

	while (1)
	{
		prompt();

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			free(line);
			exit(EXIT_SUCCESS);
		}

		argv = parse_line(line);
		if (argv == NULL)
			continue;

		execute_cmd(argv);
		free_args(argv);
	}
	return (0);
}
