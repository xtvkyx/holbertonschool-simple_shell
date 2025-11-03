#include "shell.h"

/**
 * is_builtin - Check if argv matches any builtin.
 * @argv: Argument vector.
 * Return: 1 if builtin, 0 otherwise.
 */
int is_builtin(char **argv)
{
	if (!argv || !argv[0])
		return (0);
	if (_strcmp(argv[0], "env") == 0)
		return (1);
	if (_strcmp(argv[0], "exit") == 0)
		return (1);
	return (0);
}

/**
 * builtin_env - Print environment if command is "env".
 * @argv: Argument vector.
 * Return: 0 if handled, otherwise 0 to let others run.
 */
int builtin_env(char **argv)
{
	size_t i;

	if (_strcmp(argv[0], "env") != 0)
		return (0);

	for (i = 0; environ[i]; i++)
	{
		write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
	}
	return (0);
}

/**
 * builtin_exit - Exit shell if command is "exit".
 * @argv: Argument vector (argv[1] may be a status).
 * Return: This never returns to caller if it exits; otherwise 0.
 */
int builtin_exit(char **argv)
{
	int status = 0;

	if (_strcmp(argv[0], "exit") != 0)
		return (0);

	if (argv[1])
		status = atoi(argv[1]);

	free_vector(argv);
	exit(status);
}
