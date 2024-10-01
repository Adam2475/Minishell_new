/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/01 09:41:43 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	init_execution(t_data **data, int *i)
{
	*i = 1;
	(*data)->cmd2 = NULL;
	return (0);
}

char	*find_cmd(char *cmd, t_data **data)
{
	int		i;
	char	*tmp;
	char	*holder;

	i = 0;
	while ((*data)->my_paths[i])
	{
		tmp = ft_strjoin((*data)->my_paths[i], "/");
		holder = ft_strjoin(tmp, cmd);
		if (access(holder, X_OK) == 0)
			return (free(tmp), holder);
		if (holder)
			free(holder);
		if (tmp)
			free(tmp);
		i++;
	}
	write(2, "command not found : ", 20);
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
	return (NULL);
}

t_token_list	*split_tokens_by_pipe(t_token *token_list)
{
	t_token_list	*result;
	t_token_list	*current_list;
	t_token			*start;
	t_token			*prev;

	result = NULL;
	current_list = NULL;
	start = token_list;
	prev = NULL;
	while (token_list)
	{
		if (token_list->type == TOKEN_PIPE)
		{
			terminate_segment(prev);
			current_list = create_and_link(start, result, current_list);
			free_token_segment(start);
			start = token_list->next;
		}
		prev = token_list;
		token_list = token_list->next;
	}
	if (start)
		current_list = create_and_link(start, result, current_list);
	return (result);
}

static	void	init_extraction(t_token **result, t_token **current,
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
