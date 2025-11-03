#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

/* Expose environment */
extern char **environ;

/* Core */
char *read_line(void);
char **tokenize(char *line);
int execute(char **argv);
char *resolve_path(const char *cmd);

/* Builtins */
int is_builtin(char **argv);
int builtin_env(char **argv);
int builtin_exit(char **argv);

/* Utils */
size_t _strlen(const char *s);
int _strcmp(const char *a, const char *b);
char *_strdup(const char *s);
char *str_join3(const char *a, const char *b, const char *c);
void free_vector(char **v);

#endif /* SHELL_H */
