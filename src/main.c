/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: buiterma <buiterma@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/23 20:43:40 by buiterma      #+#    #+#                 */
/*   Updated: 2022/08/24 15:16:50 by buiterma      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	main(void)
{
	char		*input_line;
	t_command	*data;

	while (validate_read(input_line))
	{
		input_line = readline(BOLD BLUE SHELL RESET);
		data = parse(input_line);
		add_history(input_line);
	}
	free (input_line);
	input_line = NULL;
	return (0);
}