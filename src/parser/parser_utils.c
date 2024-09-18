/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 19:21:15 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	exit_from_parser(t_data **data, t_token *tokens)
{
	free((*data)->command[3]);
	free_char_array((*data)->command);
	free_exit(data, tokens);
}

int	parser_case_redo(t_token *current, t_data **data)
{
	current = current->next;
	(*data)->redirect_state = 1;
	if (current->type == TOKEN_APPENDICE)
		(*data)->fd = open(current->value, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		return (1);
	return (0);
}

int	parser_case_redi(t_token *current, t_data **data)
{
	current = current->next;
	(*data)->redirect_state = 0;
	if (current->type == TOKEN_APPENDICE)
		(*data)->fd = open(current->value, O_RDONLY);
	else
		return (1);
	return (0);
}

int	parser_case_append(t_token *current, t_data **data)
{
	current = current->next;
	(*data)->redirect_state = 1;
	if (current->type == TOKEN_APPENDICE)
		(*data)->fd = open(current->value, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		return (1);
	return (0);
}

int	parser_case_herdoc(t_token *current, t_data **data)
{
	current = current->next;
	if (current->type == TOKEN_APPENDICE)
		handle_heredoc(current->value, data);
	else
		return (1);
	return (0);
}
