/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utility.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: buiterma <buiterma@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 11:20:58 by buiterma      #+#    #+#                 */
/*   Updated: 2022/09/01 12:16:56 by buiterma      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void	print_all_tokens(t_token *tokens)
{
	while (tokens != NULL)
	{
		printf("Index:		%d\n", tokens->index);
		printf("Length:		%d\n", tokens->length);
		printf("Type:		%d\n", tokens->type);
		printf("Expandable:	%d\n", tokens->expandable);
		printf("Adjacent:	%d\n\n", tokens->adjacent);
		tokens = tokens->next;
	}
}

void	print_token(t_token *token)
{
	printf("%d\n", token->index);
	printf("%d\n", token->length);
	printf("%d\n\n", token->type);
}

void	print_command(t_command *cmd, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < n)
	{
		j = 0;
		while (cmd[i].args[j])
		{
			printf("%s ", cmd[i].args[j]);
			j++;
		}
		printf("\n");
		i++;
	}
}
