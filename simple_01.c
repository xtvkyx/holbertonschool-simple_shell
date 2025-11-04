#include "simple_01.h"

#define HSH_NAME "./hsh"

/**
 * trim_whitespace - Remove leading and trailing spaces/tabs/newlines
 * @s: Input string
 * Return: Pointer to trimmed string (in the same buffer)
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

/* -------- tiny helpers for exact error format without printf -------- */

/**
 * utoa_dec - Convert unsigned long to decimal string in @buf
 * @v: value
 * @buf: output buffer (at least 32 bytes)
 * Return: pointer to start of the number inside @buf
 */
static char *utoa_dec(unsigned long v, char *buf)
{
	int i = 31;

	buf[i--] = '\0';
	if (v == 0)
		buf[i--] = '0';
	while (v > 0 && i >= 0)
	{
		buf[i--] = (char)('0' + (v % 10));
		v /= 10;
	}
	return (&buf[i + 1]);
}

/**
 * write_not_found - Print: "./hsh: N: cmd: not found\n" (checker format)
 * @count: command index starting at 1
 * @cmd: command token
 */
static void write_not_found(unsigned long count, const char *cmd)
{
	char num[32];
	char *pnum = utoa_dec(count, num);

	write(STDERR_FILENO, HSH_NAME, sizeof(HSH_NAME) - 1);
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, pnum, strlen(pnum));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, cmd, strlen(cmd));
	write(STDERR_FILENO, ": not found\n", 12);
}

/* -------------------------- main loop -------------------------- */

/**
 * shell_loop - REPL: prompt → read → parse → resolve → (fork &) exec
 * Return: last command status (used as program exit code)
 */
int shell_loop(void)
{
	char *line = NULL, *cmd, *resolved;
	size_t cap = 0;
	ssize_t n;
	char *argv_exec[MAX_ARGS];
	int last_status = 0;
	unsigned long cmd_count = 0;

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

		if (parse_input(cmd, argv_exec) == 0)
			continue;

		cmd_count++; /* count every non-empty command line */

		/* Built-ins */
                if (strcmp(argv_exec[0], "exit") == 0)
                        break;

                if (strcmp(argv_exec[0], "env") == 0)
                {
                        int i;
                        for (i = 0; environ[i]; i++)
                        { 
                                 write(STDOUT_FILENO, environ[i], strlen(environ[i]));
                                 write(STDOUT_FILENO, "\n", 1);
                        }
                        last_status = 0;
                        continue;
                }


		/* Resolve before forking; skip fork if not found */

		resolved = find_path(argv_exec[0]);
		if (!resolved)
		{
			write_not_found(cmd_count, argv_exec[0]);
			last_status = 127; /* required exit code for "not found" */
			continue;
		}

		argv_exec[0] = resolved;
		last_status = execute_child(argv_exec);
	}

	free(line);
	return (last_status);
}

/**
 * main - Entry point for the simple shell (0.3)
 * Return: Exit status from last executed command
 */
int main(void)
{
	return (shell_loop());
}
