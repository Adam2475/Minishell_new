/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 22:43:36 by mapichec          #+#    #+#             */
/*   Updated: 2024/10/29 17:06:16 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	redirect_parser(t_data **data, t_token *current, t_token **tokens)
{
	int	i;

	i = 0;
	g_err_state = 0;
	while (current != NULL)
	{
		if (current->type == TOKEN_REDIRECT_OUT)
			i = parser_case_redo(current, data);
		else if (current->type == TOKEN_REDIRECT_IN)
			i = parser_case_redi(current, data);
		else if (current->type == TOKEN_APPEND)
			i = parser_case_append(current, data);
		else if (current->type == TOKEN_HEREDOC)
			i = parser_case_herdoc(current, data, tokens);
		current = current->next;
		if ((*data)->skip_flag > 0)
			return (i);
	}
	return (i);
}

int	redirect_parser_pipe(t_data **data, t_token *current)
{
	int	i;

	i = 0;
	g_err_state = 0;
	while (current != NULL)
	{
		if (current->type == TOKEN_REDIRECT_OUT)
			i = parser_case_redo(current, data);
		else if (current->type == TOKEN_REDIRECT_IN)
			i = parser_case_redi(current, data);
		else if (current->type == TOKEN_APPEND)
			i = parser_case_append(current, data);
		else if (current->type == TOKEN_HEREDOC)
			i = parser_case_herdoc_pipe(current, data);
		current = current->next;
	}
	return (i);
}

int	parser_case_redi(t_token *current, t_data **data)
{
	current = current->next;
	while (current && (current->type == 11
			|| current->type == 10 || current->type == 9))
		current = current->next;
	if (current && (current->type == TOKEN_APPENDICE || current->type == 14))
	{
		(*data)->redirect_state_in = 1;
		(*data)->fd_in = open(current->value, O_RDONLY);
		if ((*data)->fd_in < 0)
		{
			if (errno != 0)
				perror("");
			(*data)->skip_flag = 1;
			return ((*data)->local_err_state = 1, 0);
		}
	}
	else
		return (1);
	return (0);
}
