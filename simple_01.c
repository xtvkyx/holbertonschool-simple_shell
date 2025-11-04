#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define PROMPT "$ "
#define MAX_ARGS 64

extern char **environ;

/**
	* trim_whitespace - Removes leading and trailing spaces from a string
	* @s: input string
	*
	* Return: pointer to trimmed string
	*/
char *trim_whitespace(char *s)
{
	char *end;

	if (s == NULL)
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
	* execute_command - Executes a given command using fork and execve
	* @argv_exec: tokenized command and arguments
	*
	* Return: void
	*/
void execute_command(char **argv_exec)
{
	pid_t child;
	int status;

	child = fork();
	if (child == -1)
	{
		perror("fork");
		return;
	}

	if (child == 0)
	{
		if (execve(argv_exec[0], argv_exec, environ) == -1)
		{
			dprintf(STDERR_FILENO, "%s: %s\n",
				argv_exec[0], strerror(errno));
			_exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (waitpid(child, &status, 0) == -1)
			perror("waitpid");
	}
}

/**
	* parse_input - Tokenizes the command line into arguments
	* @cmd: command string
	* @argv_exec: array to store arguments
	*
	* Return: number of arguments parsed
	*/
int parse_input(char *cmd, char **argv_exec)
{
	char *token;
	int i = 0;

	token = strtok(cmd, " \t");
	while (token != NULL && i < (MAX_ARGS - 1))
	{
		argv_exec[i++] = token;
		token = strtok(NULL, " \t");
	}
	argv_exec[i] = NULL;

	return (i);
}

/**
	* shell_loop - Repeatedly prompts user and executes commands
	*
	* Return: void
	*/
void shell_loop(void)
{
	char *line = NULL, *cmd;
	size_t linecap = 0;
	ssize_t linelen;
	char *argv_exec[MAX_ARGS];

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

		linelen = getline(&line, &linecap, stdin);
		if (linelen == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (linelen > 0 && line[linelen - 1] == '\n')
			line[linelen - 1] = '\0';

		cmd = trim_whitespace(line);

		if (cmd == NULL || *cmd == '\0')
			continue;

		if (strcmp(cmd, "exit") == 0)
			break;

		if (parse_input(cmd, argv_exec) > 0)
			execute_command(argv_exec);
	}

	free(line);
}

/**
	* main - Entry point of the simple shell
	*
	* Return: Always 0
	*/
int main(void)
{
	shell_loop();
	return (0);
}

