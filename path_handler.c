#include "simple_01.h"

/* Check if path exists and is executable */
static int is_exec(const char *p)
{
    struct stat st;

    return (p && stat(p, &st) == 0 && S_ISREG(st.st_mode)
            && access(p, X_OK) == 0);
}

/**
 * find_path - Resolve command via PATH; empty components mean "."
 * @cmd: command name or path
 * Return: pointer to static buffer with full path, or NULL if not found
 */
char *find_path(char *cmd)
{
    static char full[1024];
    char *env, *start, *colon;
    size_t len;

    if (!cmd || *cmd == '\0')
        return (NULL);

    if (strchr(cmd, '/'))
        return (is_exec(cmd) ? cmd : NULL);

    env = getenv("PATH");
    if (!env || *env == '\0')
        return (NULL);

    start = env;
    while (1)
    {
        char *dir;

        colon = strchr(start, ':');
        len = colon ? (size_t)(colon - start) : strlen(start);

        if (len == 0)
            dir = ".";
        else
        {
            if (len >= sizeof(full))
                len = sizeof(full) - 1;
            memcpy(full, start, len);
            full[len] = '\0';
            dir = full;
        }

        snprintf(full, sizeof(full), "%s/%s", dir, cmd);
        if (is_exec(full))
            return (full);

        if (!colon)
            break;
        start = colon + 1;
    }
    return (NULL);
}
