#include "simple_01.h"
int execute_child(char **argv);

/**
	* read_line - Read one line from stdin using getline.
	* Return: malloc'd buffer or NULL on EOF/error.
	*/
char *read_line(void)
{
	char *line = NULL;
	size_t cap = 0;
	ssize_t n = getline(&line, &cap, stdin);

	if (n == -1)
	{
		free(line);
		return (NULL);
	}

	if (n > 0 && line[n - 1] == '\n')
		line[n - 1] = '\0';

	return (line);
}

/**
	* first_token - Return the first non-space word.
	* @line: Buffer (modified in place).
	* Return: Pointer to first word or NULL if empty.
	*/
char *first_token(char *line)
{
	char *s, *e;

	if (!line)
		return (NULL);

	for (s = line; *s == ' ' || *s == '\t'; s++)
		;

	if (*s == '\0')
		return (NULL);

	for (e = s; *e && *e != ' ' && *e != '\t'; e++)
		;
	*e = '\0';

	return (s);
}

/**
 * split_line - Split a line into argv array (space/tab separated).
 * @line: Input string.
 * Return: NULL-terminated array of malloc'd tokens.
 */
char **split_line(char *line)
{
    char **argv = NULL;
    char *token;
    size_t count = 0, i;
    char **new_argv;

    token = strtok(line, " \t");
    while (token)
    {
        new_argv = malloc(sizeof(char *) * (count + 2));
        if (!new_argv)
            return (NULL);

        for (i = 0; i < count; i++)
            new_argv[i] = argv[i];

        free(argv);

        new_argv[count] = strdup(token);
        new_argv[count + 1] = NULL;

        argv = new_argv;
        count++;
        token = strtok(NULL, " \t");
    }

    return (argv);
}


/**
 * run_command - Fork + execve a command with arguments.
 * @cmdline: Command line string.
 * Return: Child exit status; 127/126 on failure.
 */
int run_command(const char *cmdline)
{
    int status = 0;
    char **argv = NULL;
    char *line_copy = NULL;
    size_t i;

    if (!cmdline || *cmdline == '\0')
        return (0);

    line_copy = strdup(cmdline);
    if (!line_copy)
        return (1);

    argv = split_line(line_copy);
    if (!argv || !argv[0])
    {
        free(line_copy);
        free(argv);
        return (0);
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("./shell");
        free(line_copy);
        for (i = 0; argv[i]; i++)
            free(argv[i]);
        free(argv);
        return (1);
    }

    if (pid == 0)
    {
        execve(argv[0], argv, environ);
        perror("./shell");
        _exit(errno == EACCES ? 126 : 127);
    }
    else
    {
        if (waitpid(pid, &status, 0) == -1)
            perror("./shell");
    }

    for (i = 0; argv[i]; i++)
        free(argv[i]);
    free(argv);
    free(line_copy);

    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (1);
}


/**
	* main - Simple shell 0.1: prompt (only if tty) → read → parse → run.
	* Return: Last command status or 0.
	*/
int main(void)
{
	int status = 0;
	char *line, *cmd;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		line = read_line();
		if (!line)
			break;

		cmd = first_token(line);
		if (cmd)
			status = run_command(cmd);

		free(line);
	}

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);

	return (status);
}
