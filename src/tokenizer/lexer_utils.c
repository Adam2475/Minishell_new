/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexers_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	special_cases_lexer2(t_data **data,
	char *buffer, t_token **tokens, char *end)
{

}

int	whitespace_case(char *buffer, char *end, t_token **tokens)
{
	int	i;

	i = 0;
	while (*end == WHITESPACE)
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
	int	i;

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
