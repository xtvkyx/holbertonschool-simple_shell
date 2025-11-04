/* simple_shell.c - Simple shell (supports arguments, uses execve)
 * Compile:
 * gcc -Wall -Werror -Wextra -pedantic -std=gnu89 simple_shell.c -o simple_shell
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

#define PROMPT "$ "
#define MAX_ARGS 64

static char *trim_whitespace(char *s)
{
    char *end;

    if (s == NULL)
        return NULL;

    while (*s == ' ' || *s == '\t' || *s == '\n')
        s++;

    if (*s == '\0')
        return s;

    end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\n'))
    {
        *end = '\0';
        end--;
    }
    return s;
}

int main(void)
{
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    char *argv_exec[MAX_ARGS];
    pid_t child;
    int status;
    int i;
    char *token;
    char *cmd;

    while (1)
    {
        if (isatty(STDIN_FILENO))
        {
            if (write(STDOUT_FILENO, PROMPT, strlen(PROMPT)) == -1)
            {
                /* non-fatal, continue */
            }
        }

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

        i = 0;
        token = strtok(cmd, " \t");
        while (token != NULL && i < (MAX_ARGS - 1))
        {
            argv_exec[i++] = token;
            token = strtok(NULL, " \t");
        }
        argv_exec[i] = NULL;

        if (argv_exec[0] == NULL)
            continue;

        child = fork();
        if (child == -1)
        {
            perror("fork");
            continue;
        }

        if (child == 0)
        {
            if (execve(argv_exec[0], argv_exec, environ) == -1)
            {
                dprintf(STDERR_FILENO, "%s: %s\n", argv_exec[0], strerror(errno));
                _exit(EXIT_FAILURE);
            }
        }
        else
        {
            if (waitpid(child, &status, 0) == -1)
                perror("waitpid");
        }
    }

    free(line);
    return 0;
}
