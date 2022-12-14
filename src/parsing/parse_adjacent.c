/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_adjacent.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jde-groo <jde-groo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 17:13:24 by jde-groo      #+#    #+#                 */
/*   Updated: 2022/11/23 17:05:00 by jde-groo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static bool	parse_adjacent_part(const char *input, char **res, t_token *tokens)
{
	char	*tmp;

	if (!tokens->length || tokens->length == 0xFFFFFFFF)
	{
		tmp = ft_strdup("");
		res[0] = ft_strappendfree(res[0], tmp);
		free(tmp);
		return (res[0] != NULL);
	}
	tmp = ft_substr(input, tokens->index, tokens->length);
	if (!tmp)
		return (false);
	if (tokens->expandable && !expand(&tmp))
	{
		free(tmp);
		return (false);
	}
	res[0] = ft_strappendfree(res[0], tmp);
	free(tmp);
	return (res[0] != NULL);
}

bool	parse_adjacent(const char *input, t_token *tokens, char **location)
{
	char	*res;

	res = NULL;
	while (tokens->adjacent)
	{
		if (!parse_adjacent_part(input, &res, tokens))
		{
			free(res);
			return (false);
		}
		tokens = tokens->next;
	}
	if (!parse_adjacent_part(input, &res, tokens))
	{
		free(res);
		return (false);
	}
	location[0] = res;
	return (true);
}
