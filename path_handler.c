#include "simple_01.h"

/**
 * is_exec - Check if a path exists and is executable
 * @p: Path to check
 *
 * Return: 1 if regular file and executable, 0 otherwise
 */
static int is_exec(const char *p)
{
	struct stat st;

	return (p && stat(p, &st) == 0 && S_ISREG(st.st_mode)
		&& access(p, X_OK) == 0);
}

/**
 * find_path - Resolve command using PATH (empty entries mean ".")
 * @cmd: Command name (e.g., "ls") or path (e.g., "/bin/ls")
 *
 * Return: Pointer to static buffer with full path, or NULL if not found
 */
char *find_path(char *cmd)
{
	static char full[1024];
	char dirbuf[512];
	char *env, *start, *colon;
	size_t cmdlen, dirlen;

	if (!cmd || *cmd == '\0')
		return (NULL);

	/* If command contains '/', try it directly */
	if (strchr(cmd, '/'))
		return (is_exec(cmd) ? cmd : NULL);

	env = env_get("PATH");
	if (!env || *env == '\0')
		return (NULL);

	cmdlen = strlen(cmd);
	start = env;

	while (1)
	{
		colon = strchr(start, ':');
		dirlen = colon ? (size_t)(colon - start) : strlen(start);

		if (dirlen == 0)
		{
			dirbuf[0] = '.';
			dirbuf[1] = '\0';
			dirlen = 1;
		}
		else
		{
			if (dirlen >= sizeof(dirbuf))
				dirlen = sizeof(dirbuf) - 1;
			memcpy(dirbuf, start, dirlen);
			dirbuf[dirlen] = '\0';
		}

		/* Build full = dirbuf + "/" + cmd (avoid snprintf) */
		if (dirlen + 1 + cmdlen + 1 <= sizeof(full))
		{
			memcpy(full, dirbuf, dirlen);
			full[dirlen] = '/';
			memcpy(full + dirlen + 1, cmd, cmdlen);
			full[dirlen + 1 + cmdlen] = '\0';

			if (is_exec(full))
				return (full);
		}

		if (!colon)
			break;
		start = colon + 1;
	}

	return (NULL);
}
