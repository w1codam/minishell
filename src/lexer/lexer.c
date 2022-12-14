/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jde-groo <jde-groo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/25 12:16:07 by jde-groo      #+#    #+#                 */
/*   Updated: 2022/11/23 17:06:05 by jde-groo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static bool	process_token_length(const char *input, unsigned int *index, \
	unsigned int from)
{
	if (input[from] == '\"' || input[from] == '\'')
	{
		if (input[from + *index] == input[from] && \
			input[from + *index - 1] != '\\')
			return (true);
		else
			(*index)++;
	}
	else if (input[from + *index] != ' ' && input[from + *index] != '\t' \
		&& input[from + *index] != '|' \
		&& input[from + *index] != '>' && input[from + *index] != '<' \
		&& input[from + *index] != '"' && input[from + *index] != '\'')
		(*index)++;
	else
		return (true);
	return (false);
}

static unsigned int	token_length(const char *input, unsigned int from)
{
	unsigned int	index;

	index = 0;
	if (input[from] == '\"' || input[from] == '\'')
		index++;
	if (get_type(input, from) == HEREDOC || \
		get_type(input, from) == OUTFILE_APPEND)
		return (2);
	if (ft_strchr("|<>", input[from]))
		return (1);
	while (input[from + index])
		if (process_token_length(input, &index, from))
			break ;
	if ((input[from] == '\"' || input[from] == '\'') \
		&& input[from + index] == input[from])
		index++;
	return (index);
}

bool	clear_token_list(t_token **head)
{
	t_token	*next;
	t_token	*thead;

	thead = *head;
	while (thead)
	{
		next = thead->next;
		free(thead);
		thead = next;
	}
	*head = NULL;
	return (false);
}

static void	set_token_info(const char *input, unsigned int *index, \
	unsigned int *length, bool add_length)
{
	if (add_length)
		*index = find_next(input, *index + *length);
	else
		*index = find_next(input, *index);
	*length = token_length(input, *index);
}

bool	lexer(t_token **token, const char *input)
{
	t_tokentype		types[2];
	unsigned int	index;
	unsigned int	length;

	index = 0;
	types[0] = INFILE;
	while (input[index])
	{
		set_token_info(input, &index, &length, false);
		types[1] = get_type(input, index);
		if ((types[0] == COMMAND || types[0] == ARGUMENT) \
			&& types[1] == COMMAND)
			types[1] = ARGUMENT;
		if (types[1] == INFILE || types[1] == OUTFILE || \
			types[1] == OUTFILE_APPEND || types[1] == HEREDOC)
			set_token_info(input, &index, &length, true);
		if (!part_handler(token, index, length, types[1]))
			return (clear_token_list(token));
		index += length;
		types[0] = types[1];
	}
	return (post_process(input, *token));
}
