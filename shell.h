#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for cnv_num () */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system gline() */
#define USE_GLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct passdata - contains pseudo-arguements to pass into a function,
 * allowing uniform prototype for function pointer struct
 * @arg: a string generated from gline containing arguements
 * @argv:an array of strings generated from arg
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount_flag: if on count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: the history node
 * @alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @histcount: the history line number count
 */
typedef struct passdata
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} data_t;

#define DATA_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct bulk - contains string
 * @type: command flag
 * @func: function
 */
typedef struct bulk
{
	char *type;
	int (*func)(data_t *);
} bulk_table;


/* toem_shloop.c */
int hsh(data_t *, char **);
int find_bulk(data_t *);
void find_cmd(data_t *);
void fork_cmd(data_t *);

/* toem_parser.c */
int is_cmd(data_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(data_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_error.c */
void _print_puts(char *);
int _eputchar(char);
int _putinfd(char c, int fd);
int _putiinsfd(char *str, int fd);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_ex.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bfree(void **);

/* toem_atens.c */
int interactive(data_t *);
int is_delim(char, char *);
int _alpha(int);
int _atens(char *);

/* toem_error.c */
int _erratens(char *);
void printerr(data_t *, char *);
int pr_d(int, int);
char *cnv_num (long int, int, int);
void rm_com(char *);

/* toem_bulk.c */
int _exits(data_t *);
int _cd(data_t *);
int _help(data_t *);

/* toem_bulk1.c */
int _history(data_t *);
int _malias(data_t *);

/*toem_gline.c */
ssize_t get_input(data_t *);
int _gline(data_t *, char **, size_t *);
void sHand(int);

/* toem_getdata.c */
void clear_data(data_t *);
void set_data(data_t *, char **);
void free_data(data_t *, int);

/* toem_env.c */
char *_genv(data_t *, const char *);
int _env(data_t *);
int _msenv(data_t *);
int _muenv(data_t *);
int pop_env(data_t *);

/* toem_genv.c */
char **g_env(data_t *);
int _unsenv(data_t *, char *);
int _senv(data_t *, char *, char *);

/* toem_hist.c */
char *gt_hfile(data_t *data);
int wr_his(data_t *data);
int rd_hist(data_t *data);
int b_history(data_t *data, char *buf, int linecount);
int r_history(data_t *data);

/* toem_list.c */
list_t *add_nd(list_t **, const char *, int);
list_t *add_nd_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* toem_lists1.c */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* toem_vars.c */
int is_chain(data_t *, char *, size_t *);
void check_chain(data_t *, char *, size_t *, size_t, size_t);
int replace_alias(data_t *);
int replace_vars(data_t *);
int replace_string(char **, char *);

#endif
