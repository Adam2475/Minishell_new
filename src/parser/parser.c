/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/23 11:14:23 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	call_for_command(t_token **tokens, t_data **data,
		t_token **origin)
{
	t_token		*current;
	int			i;

	i = 0;
	current = *origin;
	(*data)->command[i++] = ft_strdup(current->value);
	current = current->next;
	while (current->type == 13 || current->type == 1
		|| current->type == 8 || current->type == 11)
	{
		while (current->type == 11 || current->type == 10 || current->type == 9)
			current = current->next;
		if (current->type == TOKEN_APPENDICE || current->type == 14)
		{
			(*data)->command[i] = ft_strdup(current->value);
			current = current->next;
		}
		else
			break ;
		i++;
	}
	execute_command_single((*data)->command, data, tokens);
	if ((*data)->fd >= 0)
		close((*data)->fd);
	if ((*data)->fd_in >= 0)
		close((*data)->fd_in);
	if ((*data)->fd_out >= 0)
		close((*data)->fd_out);
	return (0);
}

static	int	token_words(t_token *token)
{
	int		i;
	t_token	*tmp;

	tmp = token;
	i = 0;
	while (tmp && tmp->type != TOKEN_EOF)
	{
		if (tmp->type == TOKEN_WHITESPACE)
			;
		else
			i++;
		tmp = tmp->next;
	}
	return (i + 1);
}

static	int	parser_init(t_data **data)
{
	size_t	words;

	words = (size_t)token_words((*data)->tokens);
	(*data)->command = (char **)ft_calloc(words + 2, sizeof(char *));
	if (!(*data)->command)
		return (1);
	return (0);
}

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
	}
	return (i);
}

int	redirect_parser_pipe(t_data **data, t_token *current, t_token **tokens)
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
			i = parser_case_herdoc_pipe(current, data, tokens);
		current = current->next;
	}
	return (i);
}

int	token_parser(t_token **tokens, t_data **data)
{
	t_token		*current;

	if (parser_init(data) > 0)
		free_exit(data);
	current = *tokens;
	while (current && current->type != TOKEN_EOF)
	{
		if ((*data)->heredoc_flag == 0)
		{
			if (redirect_parser(data, current, tokens) > 0)
			{
				write(2, "error!\n", 8);
				return (g_err_state = 2, errno = 2, strerror(errno), 1);
			}
		}
		if (current->type == 12 || current->type == 14)
		{
			if (!call_for_command(tokens, data, &current))
				return (0);
			else
				free_exit(data);
		}
		current = current->next;
	}
	if ((*data)->fd && (*data)->fd >= 0)
		close((*data)->fd);
	if ((*data)->fd_in)
		close((*data)->fd_in);
	if ((*data)->fd_out)
		close((*data)->fd_out);
	return (0);
}
