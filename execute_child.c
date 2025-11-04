#include "simple_01.h"
/**
	* execute_child - Fork and execute a given arg.
	*  @argv: Argument vector.
	*  Return: Exit status.
	*/
int execute_child(char **argv)
{
	pid_t pid;
	int status = 0;

	pid = fork();
	if (pid < 0)
	{
	perror("./shell");
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

	if (WIFEXITED(status))
	return (WEXITSTATUS(status));

	return (1);
}
