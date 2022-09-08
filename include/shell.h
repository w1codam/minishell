/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: buiterma <buiterma@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/23 16:00:11 by buiterma      #+#    #+#                 */
/*   Updated: 2022/09/08 11:44:08 by buiterma      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <stdlib.h>
# include "../libs/libft/include/libft.h"

# define SHELL "[mini_pain]> "
# define SPECIAL_CHAR "$|<>\'\""

typedef struct s_env {
	char			*key;
	char			*value;
	struct s_env	*next;
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
}	t_command;

typedef struct s_shell {
	t_env		*env;
	int			exit_code;
	int			fd_in;
	int			fd_out;
	int			pipe[2];
	pid_t		pid;
	t_command	*cmds;
	size_t		cmd_n;
}	t_shell;

t_shell			g_shell;

// int				main(int argc, char **argv, char **envp);

//============== Lexer =============//
t_token			*lexer(const char *input);
t_token			*post_process(const char *input, t_token *head);
t_tokentype		get_type(const char *input, unsigned int from);
unsigned int	find_next(const char *input, unsigned int from);
bool			add_token(t_token **head, t_tokentype type, unsigned int index, \
					unsigned int length);
bool			part_handler(t_token **head, unsigned int index, \
					unsigned int length, t_tokentype type);
t_token			*clear_token_list(t_token *head);

//============ Parsing =============//
void			print_command(t_command *cmd, size_t n);
void			print_all_tokens(t_token *tokens);
void			parse_special(t_token *tokens, char const *input);
bool			parser(t_token *tokens, char const *input);
bool			parse_commands(t_token *tokens, char const *input);

//========== Environment ==========//
t_env			*parse_environment(char **envp);
t_env			*clear_list(t_env *head);
t_env			*get_env(t_env *head, char *key);
bool			add_variable(t_env **head, char *var_string);
bool			remove_variable(t_env **head, char *key);
bool			expand(t_env *head, char **location);

//========== Built-ins ==========//
int				ft_echo(int argc, char **argv);

#endif