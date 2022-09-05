/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: buiterma <buiterma@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/23 20:43:40 by buiterma      #+#    #+#                 */
/*   Updated: 2022/09/05 10:33:59 by buiterma      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	main(void)
{
	char	*input;
	t_token	*tokens;
	t_shell	shell;

	while (1)
	{
		input = readline(BOLD BLUE SHELL RESET);
		if (!input)
			break ;
		add_history(input);
		tokens = lexer(input);
		shell = parser(tokens, input);
		if (!ft_strncmp(input, "exit", 4))
			break ;
		free (input);
		input = NULL;
	}
	return (0);
}
