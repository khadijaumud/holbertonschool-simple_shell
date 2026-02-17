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
	int last_status = 0;

	while (1)
	{
		prompt(); [cite: 1]

		read = getline(&line, &len, stdin); [cite: 1]
		if (read == -1)
		{
			free(line);
			exit(last_status);
		}

		argv = parse_line(line); [cite: 1]
		if (argv == NULL || argv[0] == NULL)
		{
			free(argv);
			continue;
		}

		last_status = execute_cmd(argv);
		free_args(argv); [cite: 1]
	}
	return (last_status);
}

/**
 * execute_cmd - executes command only if it exists in PATH
 * @argv: arguments array
 * Return: status code (0 for success, 127 for not found)
 */
int execute_cmd(char **argv)
{
	pid_t pid;
	int status, exit_status = 0;
	char *full_path;

	if (argv == NULL || argv[0] == NULL)
		return (0);

	full_path = _which(argv[0]); [cite: 1, 3]
	if (full_path == NULL)
	{
		/* Формат ошибки: shell_name: line_number: command: not found */
		fprintf(stderr, "./hsh: 1: %s: not found\n", argv[0]);
		return (127); /* Ожидаемый код для "command not found" */
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
	else
		perror("fork");

	free(full_path);
	return (exit_status);
}

