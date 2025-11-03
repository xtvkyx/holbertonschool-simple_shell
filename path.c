#include "shell.h"

/**
 * resolve_path - If cmd is not absolute/relative, search it in PATH.
 * @cmd: Command name.
 * Return: Newly allocated absolute path to executable, or NULL if not found.
 */
char *resolve_path(const char *cmd)
{
	char *path, *copy, *dir, *full;
	struct stat st;

	if (!cmd || *cmd == '\0')
		return (NULL);

	/* If contains '/' try directly */
	if (strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && (st.st_mode & S_IXUSR))
			return (_strdup(cmd));
		return (NULL);
	}

	path = getenv("PATH");
	if (!path)
		return (NULL);

	copy = _strdup(path);
	if (!copy)
		return (NULL);

	dir = strtok(copy, ":");
	while (dir)
	{
		full = str_join3(dir, "/", cmd);
		if (full && stat(full, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(copy);
			return (full);
		}
		free(full);
		dir = strtok(NULL, ":");
	}

	free(copy);
	return (NULL);
}
