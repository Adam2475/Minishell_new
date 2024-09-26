/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/23 17:34:26 by adapassa         ###   ########.fr       */
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
		if (current->type == TOKEN_APPENDICE)
		{
			(*data)->command[i] = ft_strdup(current->value);
			current = current->next;
		}
		else
			break ;
		i++;
	}
	execute_command_single((*data)->command, data, envp, tokens);
	if ((*data)->fd >= 0)
		close((*data)->fd);
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

static int	parser_init(t_data **data)
{
	size_t	words;

	words = (size_t)token_words((*data)->tokens);
	(*data)->command = (char **)ft_calloc(words, sizeof(char *));
	if (!(*data)->command)
		return (1);
	return (0);
}

int	redirect_parser(t_data **data, t_token *current)
{
	if ((*data)->fd >= 0)
		return (0);
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

	if (parser_init(data) > 0)
		free_exit(data);
	current = *tokens;
	while (current && current->type != TOKEN_EOF)
	{
		if (redirect_parser(data, current) > 0)
			free_exit(data);
		if (current->type == 12)
		{
			if (!call_for_command(tokens, data, &current, envp))
				return (0);
			else
				free_exit(data);
		}
		current = current->next;
	}
	return (0);
}
