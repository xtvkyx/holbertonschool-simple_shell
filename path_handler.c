#include <sys/stat.h>
#include "simple_01.h"

/**
	* find_path - search for command in PATH
	* @cmd: command to search for
	* Return: full path if found, otherwise NULL
	*/
char *find_path(char *cmd)
{
	char *path_env, *path_copy, *dir;
	static char full_path[1024];
	struct stat st;

	/* If the command already has '/' (like /bin/ls), return it directly */
	if (strchr(cmd, '/'))
	return (cmd);

	path_env = getenv("PATH");
	if (!path_env)
	return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
	return (NULL);

	dir = strtok(path_copy, ":");
	while (dir)
	{
	snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
	if (stat(full_path, &st) == 0 && access(full_path, X_OK) == 0)
	{
	free(path_copy);
	return (full_path);
	}
	dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

