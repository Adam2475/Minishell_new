/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 22:35:11 by mapichec          #+#    #+#             */
/*   Updated: 2024/10/24 22:41:15 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	skip_tkn_space(t_token **current)
{
	while ((*current) && (*current)->type == TOKEN_WHITESPACE)
		(*current) = (*current)->next;
}

int	find_special(char c)
{
	if (c && c != WHITESPACE && c != REDIRECT_LEFT
		&& c != PIPE && c != REDIRECT_RIGHT && c != '$'
		&& c != DOUBLE_QUOTES && c != SINGLE_QUOTES)
		return (0);
	else
		return (1);
}

void	token_builder(t_token **tokens, char *buffer, char *end, int flag)
{
	if (flag == 0)
	{
		ft_tokenadd_back(tokens, ft_lstnewtoken(1,
				ft_strndup(buffer, end - buffer)));
	}
	else
	{
		ft_tokenadd_back(tokens, ft_lstnewtoken(0,
				ft_strndup(buffer, end - buffer)));
	}
}

int	recognizer(char *buffer, t_token **tokens,
		char *end, t_data **data)
{
	while (*buffer)
	{
		end = buffer;
		if (*buffer == WHITESPACE || *buffer == '\t')
		{
			buffer = buffer + whitespace_case(buffer, end, tokens);
			continue ;
		}
		if (*buffer == '<' || *buffer == '>' || *buffer == '|' || *buffer == '$'
			|| *buffer == SINGLE_QUOTES || *buffer == DOUBLE_QUOTES)
		{
			buffer = buffer + special_cases_lexer(data, buffer, tokens, end);
			continue ;
		}
		end = buffer;
		while (*end && find_special(*end) < 1)
			end++;
		if (*buffer == '-')
			token_builder(tokens, buffer, end, 0);
		else
			token_builder(tokens, buffer, end, 1);
		buffer = end;
	}
	ft_tokenadd_back(tokens, ft_lstnewtoken(7, ft_strndup(buffer, *buffer)));
	return (0);
}

int	check_spaces(t_token *tokens)
{
	t_token	*tkn;

	tkn = tokens;
	while (tkn && tkn->type != 7)
	{
		if (tkn->type == 11)
			tkn = tkn->next;
		else
			return (0);
	}
	return (1);
}
