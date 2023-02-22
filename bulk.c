#include "shell.h"

/**
 * _exits - exits the shell
 * @data: Structure for arguments.
 * Return: exits with a given exit status
 * (0) if data.argv[0] != "exit"
 */
int _exits(data_t *data)
{
	int exitcheck;

	if (data->argv[1]) /* If there is an exit arguement */
	{
		exitcheck = _erratens(data->argv[1]);
		if (exitcheck == -1)
		{
			data->status = 2;
			print_error(data, "Illegal number: ");
			_eputs(data->argv[1]);
			_eputchar('\n');
			return (1);
		}
		data->err_num = _erratens(data->argv[1]);
		return (-2);
	}
	data->err_num = -1;
	return (-2);
}

/**
 * _cd - changes the current directory
 * @data: Structure containing potential arguments
 * Return: Always 0
 */
int _cd(data_t *data)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!data->argv[1])
	{
		dir = _getenv(data, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _getenv(data, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(data->argv[1], "-") == 0)
	{
		if (!_getenv(data, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(data, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _getenv(data, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(data->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(data, "can't cd");
		_eputs(data->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(data, "OLDPWD", _getenv(data, "PWD="));
		_setenv(data, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _help - changes the current directory of the process
 * @data: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int _help(data_t *data)
{
	char **arg_array;

	arg_array = data->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
