/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: buiterma <buiterma@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/23 16:00:11 by buiterma      #+#    #+#                 */
/*   Updated: 2022/11/23 14:03:51 by buiterma      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <stdlib.h>
# include <limits.h>
# include <signal.h>
# include <errno.h>
# include <termios.h>
# include <sys/wait.h>
# include "../libs/libft/include/libft.h"

# define READ 0
# define WRITE 1
# define SHELL "[mini_pain]> "

typedef struct s_env {
	char			*key;
	char			*value;
	struct s_env	*next;
	bool			hidden;
}	t_env;

typedef enum e_tokentype {
	INFILE,
	HEREDOC,
	OUTFILE,
	OUTFILE_APPEND,
	COMMAND,
	ARGUMENT,
	PIPE
}	t_tokentype;

typedef struct s_token {
	t_tokentype		type;
	unsigned int	index;
	unsigned int	length;
	bool			expandable;
	bool			adjacent;
	struct s_token	*next;
}	t_token;

typedef struct s_command {
	char	*path;
	char	**args;
	bool	invalid;
	int		fd_in;
	int		fd_out;
}	t_command;

typedef struct s_shell {
	t_env		*env;
	int			exit_code;
	pid_t		pid;
	t_command	*cmds;
	size_t		cmd_n;
}	t_shell;

extern t_shell	g_shell;

int				main(int argc, char **argv, char **envp);

//============== Lexer =============//
bool			lexer(t_token **token, const char *input);
bool			post_process(const char *input, t_token *head);
t_tokentype		get_type(const char *input, unsigned int from);
unsigned int	find_next(const char *input, unsigned int from);
bool			add_token(t_token **head, t_tokentype type, unsigned int index, \
					unsigned int length);
bool			part_handler(t_token **head, unsigned int index, \
					unsigned int length, t_tokentype type);
bool			clear_token_list(t_token **head);

//============ Parsing =============//
void			print_command(t_command *cmd, size_t n);
void			print_all_tokens(t_token *tokens);
bool			parse_special(t_token *tokens, char const *input);
bool			parser(t_token **token, char const *input);
bool			parse_commands(t_token *tokens, char const *input);
bool			parse_adjacent(const char *input, \
					t_token *tokens, char **location);
void			purge_commands(void);

//======== Input Parsing ========//
bool			resolve_paths(void);

//========== Environment ==========//
bool			parse_environment(char **envp);
bool			clear_list(t_env **head);
t_env			*get_env(t_env *head, char *key);
bool			add_variable(t_env **head, char *var_string);
bool			remove_variable(t_env **head, char *key);
bool			expand(char **location);
bool			add_env(t_env *node);
bool			set_env(char *key, char *value);
char			**normalize_env(void);
void			place_env(t_env **head, t_env *node);
bool			do_expand(char *location, char **expanded, unsigned int *index);

//========== Built-ins ==========//
int				cmd_echo(int argc, const char **argv);
int				cmd_cd(int argc, const char **argv);
int				cmd_env(int argc, const char **argv);
int				cmd_export(int argc, const char **argv);
int				cmd_pwd(void);
int				cmd_unset(int argc, const char **argv);
int				cmd_exit(int argc, const char **argv);

//========== Signals ===========//
void			set_signals(void);
void			init_signal(void);
void			set_sigs_exec(void);
void			sig_handler_exec(int sig);
void			sig_ignore(void);

//========== Error ============//
int				error(char *cmd, char *input, char *msg, int exit_code);

//========== Utils ============//
bool			ft_pipe(int fds[2]);
bool			ft_fork(pid_t *pid);
int				cleanup(t_token *token, int exit, bool exit_prog);
void			free_program_data(void);

//=========== Exec ============//
bool			exec_func(size_t index);
bool			exec_child(int index);
void			ft_exec(size_t index);
bool			exec(void);
bool			set_shlvl(void);

#endif