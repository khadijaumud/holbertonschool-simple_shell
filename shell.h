#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

void prompt(void);
char **parse_line(char *line);
void execute_cmd(char **argv);
void free_args(char **argv);
char *_which(char *command);

#endif
