#include "simple_01.h"

/**
 * trim_whitespace - Remove leading and trailing spaces/tabs/newlines
 * @s: Input string
 *
 * Return: Pointer to trimmed string (inside the same buffer)
 */
char *trim_whitespace(char *s)
{
	char *end;

	if (!s)
		return (NULL);

	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;

	if (*s == '\0')
		return (s);

	end = s + strlen(s) - 1;
	while (end > s && (*end == ' ' || *end == '\t' || *end == '\n'))
	{
		*end = '\0';
		end--;
	}

	return (s);
}

/**
 * parse_input - Tokenize a command line into argv-like array
 * @cmd: Input command (modified in place)
 * @argv_exec: Output array for tokens (MAX_ARGS size)
 *
 * Return: Number of tokens parsed
 */
int parse_input(char *cmd, char **argv_exec)
{
	char *token;
	int i = 0;

	token = strtok(cmd, " \t");
	while (token && i < (MAX_ARGS - 1))
	{
		argv_exec[i++] = token;
		token = strtok(NULL, " \t");
	}
	argv_exec[i] = NULL;

	return (i);
}

/**
 * shell_loop - REPL: prompt → read → parse → resolve → (fork &) exec
 *
 * Notes:
 * - No fork when command is not found (resolve first).
 * - Supports arguments.
 * - Works in interactive and non-interactive modes.
 */
void shell_loop(void)
{
	char *line = NULL, *cmd, *resolved;
	size_t cap = 0;
	ssize_t n;
	char *argv_exec[MAX_ARGS];

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);

		n = getline(&line, &cap, stdin);
		if (n == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (n > 0 && line[n - 1] == '\n')
			line[n - 1] = '\0';

		cmd = trim_whitespace(line);
		if (!cmd || *cmd == '\0')
			continue;

		/* example: builtins can be added later; not required in 0.3 */
		if (strcmp(cmd, "exit") == 0)
			break;

		if (parse_input(cmd, argv_exec) > 0)
		{
			resolved = find_path(argv_exec[0]);
			if (!resolved)
			{
				/* "<cmd>: not found\n" without dprintf */
				write(STDERR_FILENO, argv_exec[0], strlen(argv_exec[0]));
				write(STDERR_FILENO, ": not found\n", 12);
				continue;
			}
			argv_exec[0] = resolved;
			(void)execute_child(argv_exec);
		}
	}

	free(line);
}

/**
 * main - Entry point for the simple shell (0.3)
 *
 * Return: Exit status from last executed command or 0
 */
int main(void)
{
	shell_loop();
	return (0);
}
