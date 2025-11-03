#include "shell_0_1.h"

/**
 * print_prompt - Print interactive prompt if stdin is a terminal.
 */
void print_prompt(void)
{
	const char *p = "#cisfun$ ";

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, p, 9);
}

/**
 * read_line - Read one line from stdin using getline.
 * Return: malloc'd line, or NULL on EOF/error.
 */
char *read_line(void)
{
	char *line = NULL;
	size_t cap = 0;
	ssize_t nread = getline(&line, &cap, stdin);

	if (nread == -1)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

/**
 * strip_newline - Replace trailing '\n' with '\0' if present.
 * @s: String buffer.
 */
void strip_newline(char *s)
{
	size_t n;

	if (!s)
		return;

	n = strlen(s);
	if (n && s[n - 1] == '\n')
		s[n - 1] = '\0';
}

/**
 * first_token - Extract first word (no arguments allowed in task 0.1).
 * @line: Input buffer (modified).
 * Return: Pointer to first token or NULL.
 */
char *first_token(char *line)
{
	char *start, *end;

	if (!line)
		return (NULL);

	start = line;
	while (*start == ' ' || *start == '\t')
		start++;

	if (*start == '\0')
		return (NULL);

	end = start;
	while (*end && *end != ' ' && *end != '\t')
		end++;

	*end = '\0';
	return (start);
}

/**
 * run_command - Fork and execve a one-word command (no PATH search).
 * @cmd: Command path (absolute/relative).
 * Return: Child exit status (0..255), or 127/126 on failure.
 */
int run_command(const char *cmd)
{
	pid_t pid;
	int status = 0;
	char *argv[2];

	if (!cmd || *cmd == '\0')
		return (0);

	argv[0] = (char *)cmd;
	argv[1] = NULL;

	pid = fork();
	if (pid < 0)
	{
		perror("./shell");
		return (1);
	}
	if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			perror("./shell");
			_exit(errno == EACCES ? 126 : 127);
		}
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			perror("./shell");
	}

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/**
 * shell_loop - Main REPL loop: prompt → read → parse → run.
 * Return: Last command status or 0 if none.
 */
int shell_loop(void)
{
	char *line, *cmd;
	int status = 0;

	while (1)
	{
		print_prompt();

		line = read_line();
		if (!line)
			break;

		strip_newline(line);
		cmd = first_token(line);

		if (cmd && *cmd)
			status = run_command(cmd);

		free(line);
	}

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	return (status);
}

/**
 * main - Entry for simple shell (task 0.1).
 * Return: Exit status from shell_loop.
 */
int main(void)
{
	return (shell_loop());
}
