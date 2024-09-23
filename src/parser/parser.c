/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/23 13:55:03 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	call_for_command(t_token **tokens, t_data **data,
		t_token **origin, char **envp)
{
	t_token		*current;
	int			i;

	i = 0;
	current = *origin;
	(*data)->command[i] = ft_strdup(current->value);
	i++;
	current = current->next;
	while (current->type == 13 || current->type == 1 ||
		current->type == 8 || current->type == TOKEN_WHITESPACE)
	{
		if (current->type == TOKEN_WHITESPACE)
			current = current->next;
		(*data)->command[i] = ft_strdup(current->value);
		current = current->next;
		i++;
	}
	execute_command_single((*data)->command, data, envp, tokens);
	if ((*data)->fd >= 0)
		close((*data)->fd);
	return (0);
}

static int	parser_init(t_data **data)
{
	(*data)->command = (char **)ft_calloc(4, sizeof(char *));
	if (!(*data)->command)
		return (1);
	return (0);
}

static	int	redirect_parser(t_data **data, t_token *current)
{
	while (current != NULL)
	{
		if (current->type == TOKEN_REDIRECT_OUT)
			return (parser_case_redo(current, data));
		else if (current->type == TOKEN_REDIRECT_IN)
			return (parser_case_redi(current, data));
		else if (current->type == TOKEN_APPEND)
			return (parser_case_append(current, data));
		else if (current->type == TOKEN_HEREDOC)
			return (parser_case_herdoc(current, data));
		else
			current = current->next;
	}
	return (0);
}

int	token_parser(t_token **tokens, t_data **data, char **envp)
{
	t_token		*current;
	t_token		*head;

	if (parser_init(data) > 0)
		exit_from_parser(data, *tokens);
	current = *tokens;
	head = *tokens;
	while (current && current->type != TOKEN_EOF)
	{
		if (redirect_parser(data, current) > 0)
			exit_from_parser(data, *tokens);
		current = head;
		if (current->type == 12)
		{
			if (!call_for_command(tokens, data, &current, envp))
				return (0);
			else
				exit_from_parser(data, *tokens);
		}
		current = current->next;
	}
	return (0);
}
