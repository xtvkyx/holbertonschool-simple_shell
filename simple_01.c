#include "simple_01.h"

/**
 * read_line - read one line from stdin (getline).
 * Return: malloc'ed buffer or NULL on EOF/error.
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
	/* drop trailing newline for easier parsing */
	if (n > 0 && line[n - 1] == '\n')
		line[n - 1] = '\0';
	return (line);
}

/**
 * first_token - return first non-space word; task 0.1 forbids arguments.
 * @line: buffer (modified in place).
 * Return: pointer to first word or NULL if only spaces/empty.
 */
char *first_token(char *line)
{
	char *s, *e;

	if (!line)
		return (NULL);

	/* skip leading spaces/tabs */
	for (s = line; *s == ' ' || *s == '\t'; s++)
		;

	if (*s == '\0')
		return (NULL);

	/* cut at first space/tab to enforce "one word only" */
	for (e = s; *e && *e != ' ' && *e != '\t'; e++)
		;
	*e = '\0';
	return (s);
}

/**
 * run_command - fork + execve the single-word command (no PATH lookup).
 * @cmd: absolute/relative path.
 * Return: child's exit status; 127/126 on failure.
 */
int run_command(const char *cmd)
{
	pid_t pid;
	int status = 0;
	char *argv[2];

	if (!cmd || !*cmd)
		return (0);

	argv[0] = (char *)cmd;
	argv[1] = NULL;

	pid = fork();
	if (pid < 0)
	{
		perror("./hsh");
		return (1);
	}
	if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			perror("./hsh");
			_exit(errno == EACCES ? 126 : 127);
		}
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			perror("./hsh");
	}

	return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}

/**
 * main - simple shell 0.1: prompt (only if tty) → read → first word → run.
 * Return: last command status or 0.
 */
int main(void)
{
	int status = 0;
	char *line, *cmd;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		line = read_line();          /* handles EOF (Ctrl+D / pipe end) */
		if (!line)
			break;

		cmd = first_token(line);     /* ignores spaces-only lines */
		if (cmd)
			status = run_command(cmd);

		free(line);
	}

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	return (status);
}
