#include "simple_01.h"

/**
 * trim_whitespace - Remove leading and trailing whitespace from a string
 * @s: Input string
 * Return: Pointer to trimmed string
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
 * parse_input - Split a command line into arguments
 * @cmd: Input command string
 * @argv_exec: Array to store argument tokens
 * Return: Number of arguments
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
 * shell_loop - Main interactive shell loop
 * Handles prompt, reading, parsing, PATH lookup, and execution
 */
void shell_loop(void)
{
    char *line = NULL, *cmd, *resolved;
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
        if (!cmd || *cmd == '\0')
            continue;

        if (strcmp(cmd, "exit") == 0)
            break;

        if (parse_input(cmd, argv_exec) > 0)
        {
            /* Resolve PATH before fork */
            resolved = find_path(argv_exec[0]);
            if (!resolved)
            {
                dprintf(STDERR_FILENO, "%s: not found\n", argv_exec[0]);
                continue;
            }
            argv_exec[0] = resolved;
            (void)execute_child(argv_exec);
        }
    }

    free(line);
}

/**
 * main - Entry point of the simple shell
 * Return: Always 0
 */
int main(void)
{
    shell_loop();
    return (0);
}
