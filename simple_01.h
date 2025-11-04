#ifndef SIMPLE_01_H
#define SIMPLE_01_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define PROMPT "$ "
#define MAX_ARGS 64

extern char **environ;

char *trim_whitespace(char *s);
int   parse_input(char *cmd, char **argv_exec);
void  shell_loop(void);

char *find_path(char *cmd);
int   execute_child(char **argv);

#endif /* SIMPLE_01_H */
