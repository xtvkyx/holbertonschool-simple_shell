#include "shell.h"

/**
 * _strlen - Length of string.
 * @s: String.
 * Return: Length.
 */
size_t _strlen(const char *s)
{
	size_t n = 0;

	if (!s)
		return (0);
	while (s[n])
		n++;
	return (n);
}

/**
 * _strcmp - Lexicographic compare.
 * @a: First string.
 * @b: Second string.
 * Return: 0 if equal, <0 if a<b, >0 if a>b.
 */
int _strcmp(const char *a, const char *b)
{
	size_t i = 0;

	if (!a || !b)
		return (a == b ? 0 : (a ? 1 : -1));

	while (a[i] && b[i] && a[i] == b[i])
		i++;
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}

/**
 * _strdup - Duplicate string to new buffer.
 * @s: String.
 * Return: Pointer to new copy or NULL.
 */
char *_strdup(const char *s)
{
	size_t n, i;
	char *p;

	if (!s)
		return (NULL);
	n = _strlen(s);
	p = malloc(n + 1);
	if (!p)
		return (NULL);
	for (i = 0; i < n; i++)
		p[i] = s[i];
	p[n] = '\0';
	return (p);
}

/**
 * str_join3 - Join 3 pieces into new string: a + b + c.
 * @a: First piece.
 * @b: Second piece.
 * @c: Third piece.
 * Return: Newly allocated concatenation or NULL.
 */
char *str_join3(const char *a, const char *b, const char *c)
{
	size_t na = _strlen(a), nb = _strlen(b), nc = _strlen(c);
	size_t n = na + nb + nc;
	char *p = malloc(n + 1);
	size_t i = 0, j;

	if (!p)
		return (NULL);

	for (j = 0; j < na; j++, i++)
		p[i] = a[j];
	for (j = 0; j < nb; j++, i++)
		p[i] = b[j];
	for (j = 0; j < nc; j++, i++)
		p[i] = c[j];
	p[i] = '\0';
	return (p);
}

/**
 * free_vector - Free a NULL-terminated vector of strings.
 * @v: Vector.
 */
void free_vector(char **v)
{
	size_t i;

	if (!v)
		return;
	for (i = 0; v[i]; i++)
		free(v[i]);
	free(v);
}
