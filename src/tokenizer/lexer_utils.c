/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 11:07:13 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*token_reformatting_pipe(t_token *current)
{
	current = current->next;
	skip_tkn_space(&current);
	if (current && current->type == TOKEN_WORD)
	{
		current->type = TOKEN_COMMAND;
		current = current->next;
	}
	skip_tkn_space(&current);
	if (current && current->next && current->type != TOKEN_WORD)
		current = current->next;
	while ((current && current->type == TOKEN_WORD)
		|| current->type == TOKEN_WHITESPACE)
	{
		while (current && current->type != 7 && current->type == 11)
			current = current->next;
		if (current->type < 2 || current->type > 10)
			current->type = TOKEN_APPENDICE;
		if (current && current->next && current->type != TOKEN_PIPE)
			current = current->next;
	}
	if (current->type == 9 || current->type == 10)
		current = current->next;
	return (current);
}

t_token	*token_reformatting_command(t_token *current)
{
	current->type = TOKEN_COMMAND;
	if (current->next != NULL)
		current = current->next;
	skip_tkn_space(&current);
	while ((current && current->type == 0 && current->type != 7)
		|| current->type == TOKEN_OPTION || current->type == TOKEN_WHITESPACE
		|| current->type == TOKEN_SINGLE_QUOTES
		|| current->type == TOKEN_DOUBLE_QUOTES)
	{
		while (current->type == TOKEN_WHITESPACE)
			current = current->next;
		if (current->type == TOKEN_WORD || current->type == TOKEN_OPTION)
			current->type = TOKEN_APPENDICE;
		if (current->next && current->next->type != 7 && current->type != 2)
			current = current->next;
		else if (current->next && current->next->type == TOKEN_EOF)
			current = current->next;
	}
	if (current->type != TOKEN_DOLLAR && current->type != TOKEN_REDIRECT_IN
		&& current->type != TOKEN_REDIRECT_OUT && current->type != TOKEN_APPEND
		&& current->type != TOKEN_HEREDOC)
		current = current->next;
	return (current);
}

static	int	special_cases_lexer2(char *buffer, t_token **tokens, char *end)
{
	int	i;

	if (*buffer == DOLLAR_SIGN)
	{
		i = 1;
		end = buffer;
		while (*++end && *end != WHITESPACE
			&& *end != REDIRECT_LEFT && *end != PIPE
			&& *end != REDIRECT_RIGHT && *end != '$'
			&& *end != DOUBLE_QUOTES
			&& *end != SINGLE_QUOTES)
			i++;
		ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_DOLLAR,
				ft_strndup(buffer, end - buffer)));
		return (i);
	}
	if (*buffer == SINGLE_QUOTES)
		return (ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_SINGLE_QUOTES,
					ft_strndup(buffer, 1))), 1);
	if (*buffer == DOUBLE_QUOTES)
		return (ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_DOUBLE_QUOTES,
					ft_strndup(buffer, 1))), 1);
	return (0);
}

int	whitespace_case(char *buffer, char *end, t_token **tokens)
{
	int	i;

	i = 0;
	while (*end == WHITESPACE || *end == '\t')
	{
		end++;
		i++;
	}
	ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_WHITESPACE,
			ft_strndup(buffer, end - buffer)));
	buffer = end;
	return (i);
}

int	special_cases_lexer(t_data **data,
	char *buffer, t_token **tokens, char *end)
{
	if (*buffer == REDIRECT_LEFT && data)
	{
		if (*(buffer + 1) == REDIRECT_LEFT)
			return (ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_HEREDOC,
						ft_strndup(buffer, 2))), 2);
		else
			return (ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_REDIRECT_IN,
						ft_strndup(buffer, 1))), 1);
	}
	if (*buffer == REDIRECT_RIGHT)
	{
		if (*(buffer + 1) == REDIRECT_RIGHT)
			return (ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_APPEND,
						ft_strndup(buffer, 2))), 2);
		else
			return (ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_REDIRECT_OUT,
						ft_strndup(buffer, 1))), 1);
	}
	if (*buffer == PIPE)
	{
		end = buffer;
		return (ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_PIPE,
					ft_strndup(buffer, 1))), 1);
	}
	return (special_cases_lexer2(buffer, tokens, end));
}
