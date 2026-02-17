/**
 * pathfinder - finds the full path of a command in PATH
 * @cmd: command name entered by the user
 * @command: arguments array
 * @envp: environment variables
 * Return: command with full path or NULL if not found
 */
char **pathfinder(char *cmd, char **command, char **envp)
{
    char *current_path, *temp_path, *path_tok;
    size_t arglen = strlen(cmd);
    fullpath = NULL;

    /* If command contains '/' (absolute or relative path) */
    if (strchr(cmd, '/') != NULL && access(cmd, F_OK) == 0)
    {
        command[0] = cmd;
        return command;
    }

    /* Get PATH from environment */
    current_path = _getenv("PATH", envp);
    if (!current_path)
        return NULL;

    temp_path = strdup(current_path);
    path_tok = strtok(temp_path, ":");

    /* Iterate over each directory in PATH */
    while (path_tok)
    {
        fullpath = malloc(arglen + strlen(path_tok) + 2);
        sprintf(fullpath, "%s/%s", path_tok, cmd);

        /* Check if file exists */
        if (access(fullpath, F_OK) == 0)
        {
            command[0] = fullpath;
            free(temp_path);
            return command;
        }

        free(fullpath);
        path_tok = strtok(NULL, ":");
    }

    free(temp_path);
    return NULL; /* command not found */
}

/**
 * execute - executes the command
 * @command: arguments array
 * @envp: environment variables
 * Return: 0 if success, -1 if error
 */
int execute(char *const command[], char **envp)
{
    pid_t id;
    int status;
    char **temp = pathfinder(command[0], (char **) command, envp);

    /* fork is only called if command is found */
    if (temp != NULL)
    {
        id = fork();
        if (id < 0)
        {
            perror("fork failed");
            return -1;
        }
        else if (id == 0)
        {
            execve(temp[0], command, envp);
            exit(EXIT_FAILURE); /* execve failed */
        }
        wait(&status);
        free(fullpath);
    }
    else
    {
        /* command not found — fork is NOT called */
        fprintf(stderr, "./hsh: 1: %s: not found\n", command[0]);
        exit(127);
    }
    return 0;
}
