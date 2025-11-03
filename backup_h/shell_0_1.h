#ifndef SHELL_0_1_H
#define SHELL_0_1_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* expose environment to execve (ضعها في الهيدر وليس .c) */
extern char **environ;

/* prototypes */
void print_prompt(void);
char *read_line(void);
void strip_newline(char *s);
char *first_token(char *line);
int run_command(const char *cmd);
int shell_loop(void);

#endif /* SHELL_0_1_H */
