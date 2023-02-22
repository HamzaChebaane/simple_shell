#include "shell.h"

/**
 * _env - prints the current environment
 * @data: Structure containing potential arguments
 * Return: Always 0
 */
int _env(data_t *data)
{
	print_list_str(data->env);
	return (0);
}

/**
 * _genv - gets the value of an environ variable
 * @data: Structure containing potential arguments
 * @name: env var name
 *
 * Return: the value
 */
char *_genv(data_t *data, const char *name)
{
	list_t *node = data->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _msenv - Initialize a new environment variable
 * @data: Structure containing potential arguments
 *  Return: Always 0
 */
int _msenv(data_t *data)
{
	if (data->argc != 3)
	{
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (_senv(data, data->argv[1], data->argv[2]))
		return (0);
	return (1);
}

/**
 * _muenv - Remove an environment variable
 * @data: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int _muenv(data_t *data)
{
	int i;

	if (data->argc == 1)
	{
		_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= data->argc; i++)
		_unsenv(data, data->argv[i]);

	return (0);
}

/**
 * pop_env - populates env linked list
 * @data: Structure containing potential arguments.
 * Return: Always 0
 */
int pop_env(data_t *data)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	data->env = node;
	return (0);
}
