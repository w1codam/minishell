/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_commands.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: buiterma <buiterma@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 13:30:56 by buiterma      #+#    #+#                 */
/*   Updated: 2022/11/23 17:05:31 by jde-groo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static size_t	command_counter(t_token *tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (0);
	while (tokens)
	{
		if (tokens->type == PIPE)
			i++;
		tokens = tokens->next;
	}
	return (i + 1);
}

static size_t	arg_counter(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens->type != PIPE)
	{
		if ((tokens->type == ARGUMENT || tokens->type == COMMAND) \
			&& !tokens->adjacent)
			i++;
		tokens = tokens->next;
	}
	return (i);
}

static char	**parse_args(char const *input, t_token *tokens, size_t amount)
{
	char	**args;
	size_t	i;

	i = 0;
	args = ft_calloc(amount + 1, sizeof(char *));
	if (!args)
		return (NULL);
	while (tokens && i < amount)
	{
		while (tokens && (tokens->type != ARGUMENT && tokens->type != COMMAND))
			tokens = tokens->next;
		if (!parse_adjacent(input, tokens, &args[i]))
			return (ft_freearray(args));
		while (tokens->adjacent)
			tokens = tokens->next;
		i++;
		tokens = tokens->next;
	}
	return (args);
}

bool	parse_commands(t_token *tokens, char const *input)
{
	size_t	i;
	size_t	arg_count;

	i = 0;
	g_shell.cmd_n = command_counter(tokens);
	g_shell.cmds = ft_calloc(g_shell.cmd_n, sizeof(t_command));
	if (!g_shell.cmds)
		return (false);
	while (tokens && i < g_shell.cmd_n)
	{
		g_shell.cmds[i].fd_in = STDIN_FILENO;
		g_shell.cmds[i].fd_out = STDOUT_FILENO;
		arg_count = arg_counter(tokens);
		if (arg_count)
		{
			g_shell.cmds[i].args = parse_args(input, tokens, arg_count);
			if (!g_shell.cmds[i].args)
				return (false);
		}
		while (tokens && tokens->next && tokens->type != PIPE)
			tokens = tokens->next;
		tokens = tokens->next;
		i++;
	}
	return (true);
}
