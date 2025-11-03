#include "shell.h"

/**
 * main - Entry point for simple shell.
 * Return: Exit status of the last command, or 0 on EOF without commands.
 */
int main(void)
{
	char *line = NULL;
	char **argv = NULL;
	int status = 0;
	int interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "($) ", 4);

		line = read_line();
		if (!line) /* EOF */
			break;

		argv = tokenize(line);
		free(line);

		if (!argv || !argv[0])
		{
			free_vector(argv);
			continue;
		}

		if (is_builtin(argv))
			status = (builtin_env(argv) || builtin_exit(argv));
		else
			status = execute(argv);

		free_vector(argv);
	}

	return (status);
}
