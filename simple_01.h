#ifndef SIMPLE_01_H
#define SIMPLE_01_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

/* api */
char *read_line(void);
char *first_token(char *line);
int   run_command(const char *cmd);

#endif /* SIMPLE_01_H */
