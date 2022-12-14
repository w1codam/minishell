/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jde-groo <jde-groo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/02 11:54:27 by jde-groo      #+#    #+#                 */
/*   Updated: 2022/11/01 13:23:48 by jde-groo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "./../../include/shell.h"

bool	parse_environment(char **envp)
{
	if (!envp)
		return (false);
	while (*envp)
	{
		if (!add_variable(&g_shell.env, *envp))
			return (clear_list(&g_shell.env));
		envp++;
	}
	return (true);
}

bool	expand(char **location)
{
	unsigned int	index;
	char			*expanded;

	index = 0;
	expanded = NULL;
	while (do_expand(location[0], &expanded, &index))
		(void)"skrrt skrrt";
	if (!expanded)
		return (false);
	free(location[0]);
	location[0] = expanded;
	return (true);
}
