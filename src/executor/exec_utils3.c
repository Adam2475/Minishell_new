/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:29:18 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/07 15:15:46 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	init_extraction(t_token **result, t_token **current,
	t_data **data, t_token *tokens)
{
	(*result) = NULL;
	(*data)->command_found = 0;
	(*current) = tokens;
}

t_token	*extract_command_and_appendices(t_data **data, t_token *tokens)
{
	t_token		*result;
	t_token		*current;

	init_extraction(&result, &current, data, tokens);
	while (current)
	{
		if (current->type == TOKEN_WHITESPACE)
		{
			current = current->next;
			continue ;
		}
		if (current->type == TOKEN_COMMAND)
		{
			(*data)->command_found = 1;
			append_token(&result, create_token(current->type, current->value));
		}
		else if ((*data)->command_found && (current->type == 13
				|| current->type == 1))
			append_token(&result, create_token(current->type, current->value));
		else if ((*data)->command_found)
			break ;
		current = current->next;
	}
	return (result);
}

int	init_execution(t_data **data, int *i)
{
	*i = 1;
	(*data)->cmd2 = NULL;
	return (0);
}

int	compare_path(char *str)
{
	if (str[0] == '/')
		return (1);
	return (0);
}

void	append_token(t_token **list, t_token *new_token)
{
	t_token	*temp;

	if (!*list)
	{
		*list = new_token;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
}