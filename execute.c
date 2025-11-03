#include "shell.h"

/**
 * execute - Fork and exec the given argv using PATH resolution.
 * @argv: Null-terminated argument vector (argv[0] is command).
 * Return: Child exit status, or 127 if command not found, 126 on exec error.
 */
int execute(char **argv)
{
	pid_t pid;
	int status = 0;
	char *path = resolve_path(argv[0]);

	if (!path)
		return (127);

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(path);
		return (1);
	}
	else if (pid == 0)
	{
		if (execve(path, argv, environ) == -1)
		{
			perror(argv[0]);
			free(path);
			_exit(errno == EACCES ? 126 : 1);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	free(path);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
