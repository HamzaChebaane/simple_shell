#include "shell.h"

/**
 * hsh - main shell loop
 * @data: the parameter & return data struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(data_t *data, char **av)
{
	ssize_t r = 0;
	int bulk_ret = 0;

	while (r != -1 && bulk_ret != -2)
	{
		clear_data(data);
		if (interactive(data))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		r = get_input(data);
		if (r != -1)
		{
			set_data(data, av);
			bulk_ret = find_bulk(data);
			if (bulk_ret == -1)
				find_cmd(data);
		}
		else if (interactive(data))
			_putchar('\n');
		free_data(data, 0);
	}
	wr_his(data);
	free_data(data, 1);
	if (!interactive(data) && data->status)
		exit(data->status);
	if (bulk_ret == -2)
	{
		if (data->err_num == -1)
			exit(data->status);
		exit(data->err_num);
	}
	return (bulk_ret);
}

/**
 * find_bulk - finds a command
 * @data: the parameter & return data struct
 *
 * Return: -1 if bulk not found,
 * 0 if bulk executed successfully,
 * 1 if bulk found but not successful,
 * 2 if bulk signals exit()
 */
int find_bulk(data_t *data)
{
	int i, built_in_ret = -1;
	bulk_table bulktbl[] = {
		{"exit", _exits},
		{"env", _env},
		{"help", _help},
		{"history", _history},
		{"senv", _msenv},
		{"unsenv", _muenv},
		{"cd", _cd},
		{"alias", _malias},
		{NULL, NULL}
	};

	for (i = 0; bulktbl[i].type; i++)
		if (_strcmp(data->argv[0], bulktbl[i].type) == 0)
		{
			data->line_count++;
			built_in_ret = bulktbl[i].func(data);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @data: the parameter & return data struct
 *
 * Return: void
 */
void find_cmd(data_t *data)
{
	char *path = NULL;
	int i, k;

	data->path = data->argv[0];
	if (data->linecount_flag == 1)
	{
		data->line_count++;
		data->linecount_flag = 0;
	}
	for (i = 0, k = 0; data->arg[i]; i++)
		if (!is_delim(data->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(data, _genv(data, "PATH="), data->argv[0]);
	if (path)
	{
		data->path = path;
		fork_cmd(data);
	}
	else
	{
		if ((interactive(data) || _genv(data, "PATH=")
					|| data->argv[0][0] == '/') && is_cmd(data, data->argv[0]))
			fork_cmd(data);
		else if (*(data->arg) != '\n')
		{
			data->status = 127;
			printerr(data, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @data: the parameter & return data struct
 *
 * Return: void
 */
void fork_cmd(data_t *data)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(data->path, data->argv, g_env(data)) == -1)
		{
			free_data(data, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(data->status));
		if (WIFEXITED(data->status))
		{
			data->status = WEXITSTATUS(data->status);
			if (data->status == 126)
				printerr(data, "Permission denied\n");
		}
	}
}
