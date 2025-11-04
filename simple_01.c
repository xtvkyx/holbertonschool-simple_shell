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
	size_t count = 0;

	token = strtok(line, " \t");
	while (token)
	{
		argv = realloc(argv, sizeof(char *) * (count + 2));
		if (!argv)
			return (NULL);

		argv[count] = strdup(token);
		count++;
		token = strtok(NULL, " \t");
	}

	if (argv)
		argv[count] = NULL;

	return (argv);
}

/**
	* run_command - Fork and execve a command with arguments.
	* @cmdline: Full command line input.
	* Return: Child exit status (0–255), or 127/126 on failure.
	*/
int run_command(const char *cmdline)
{
	int status = 0;
	char **argv;
	char *line_copy;

	if (!cmdline || *cmdline == '\0')
		return (0);

	line_copy = strdup(cmdline);
	argv = split_line(line_copy);
	if (!argv || !argv[0])
	{
		free(line_copy);
		free(argv);
		return (0);
	}

	status = execute_child(argv);

	for (size_t i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);
	free(line_copy);

	return (status);
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
