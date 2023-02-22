#include "shell.h"

/**
 * gt_hfile - gets the history file
 * @data: parameter struct
 *
 * Return: allocated string containg history file
 */

char *gt_hfile(data_t *data)
{
	char *buf, *dir;

	dir = _genv(data, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * wr_his - creates a file, or appends to an existing file
 * @data: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int wr_his(data_t *data)
{
	ssize_t fd;
	char *filename = gt_hfile(data);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = data->history; node; node = node->next)
	{
		_putiinsfd(node->str, fd);
		_putinfd('\n', fd);
	}
	_putinfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * rd_hist - reads history from file
 * @data: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int rd_hist(data_t *data)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = gt_hfile(data);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			b_history(data, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		b_history(data, buf + last, linecount++);
	free(buf);
	data->histcount = linecount;
	while (data->histcount-- >= HIST_MAX)
		delete_node_at_index(&(data->history), 0);
	r_history(data);
	return (data->histcount);
}

/**
 * b_history - adds entry to a history linked list
 * @data: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int b_history(data_t *data, char *buf, int linecount)
{
	list_t *node = NULL;

	if (data->history)
		node = data->history;
	add_node_end(&node, buf, linecount);

	if (!data->history)
		data->history = node;
	return (0);
}

/**
 * r_history - renumbers the history linked list after changes
 * @data: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int r_history(data_t *data)
{
	list_t *node = data->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (data->histcount = i);
}
