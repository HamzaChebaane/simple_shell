#include "shell.h"

/**
 * _history - displays the history list
 * @data: Structure
 *  Return: Always 0
 */
int _history(data_t *data)
{
	print_list(data->history);
	return (0);
}

/**
 * u_alias - sets alias to string
 * @data: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int u_alias(data_t *data, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(data->alias),
		get_node_index(data->alias, node_starts_with(data->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * s_alias - sets alias to string
 * @data: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int s_alias(data_t *data, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (u_alias(data, str));

	u_alias(data, str);
	return (add_node_end(&(data->alias), str, 0) == NULL);
}

/**
 * p_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int p_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
		_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _malias - mimics the alias bulk (man alias)
 * @data: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _malias(data_t *data)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (data->argc == 1)
	{
		node = data->alias;
		while (node)
		{
			p_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; data->argv[i]; i++)
	{
		p = _strchr(data->argv[i], '=');
		if (p)
			s_alias(data, data->argv[i]);
		else
			p_alias(node_starts_with(data->alias, data->argv[i], '='));
	}

	return (0);
}
