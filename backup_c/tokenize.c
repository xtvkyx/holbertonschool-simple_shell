#include"shell.h"
/**
 * tokenize - Split a line into an argv-style array.
 * @line: Input string (will not be modified).
 * Return: Null-terminated vector of tokens, or NULL on failure.
 */
char **tokenize(char *line)
{
	char *copy, *tok;
	char **vec = NULL;
	size_t cap = 0, len = 0;
	const char *delim = " \t";

	if (!line)
		return (NULL);
	copy = _strdup(line);
	if (!copy)
		return (NULL);
	tok = strtok(copy, delim);
	while (tok)
	{
		if (len + 2 > cap)
		{
			size_t ncap = cap ? cap * 2 : 8;
			char **tmp = realloc(vec, sizeof(*vec) * ncap);

			if (!tmp)
			{
				free(copy);
				free_vector(vec);
				return (NULL);
			}
			vec = tmp;
			cap = ncap;
		}
		vec[len++] = _strdup(tok);
		tok = strtok(NULL, delim);
	}

	if (vec)
		vec[len] = NULL;
	free(copy);
	return (vec);
}
