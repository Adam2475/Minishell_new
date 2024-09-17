/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexers_utils.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int		whitespace_case(char *buffer, char *end, t_token **tokens)
{
	int	i;

	i = 0;
	while (*end == WHITESPACE)
	{
		end++;
		i++;
	}
	ft_tokenadd_back(&tokens, ft_lstnewtoken(TOKEN_WHITESPACE,
			ft_strndup(buffer, end - buffer)));
	buffer = end;
	return (i);
}

int	special_cases_lexer(t_data **data, char *buffer, t_token **tokens)
{
	char *end = NULL;
	if (*buffer == REDIRECT_LEFT && data)
	{
		if (*(buffer + 1) == REDIRECT_LEFT)
		{
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_HEREDOC, ft_strndup(buffer, 2)));
			return (2);
		}
		else
		{
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_REDIRECT_IN, ft_strndup(buffer, 1)));
			return (1);
		}
	}
	if (*buffer  == REDIRECT_RIGHT)
	{
		if (*(buffer + 1)  == REDIRECT_RIGHT)
		{
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_APPEND, ft_strndup(buffer, 2)));
			return (2);
		}
		else
		{
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_REDIRECT_OUT, ft_strndup(buffer, 1)));
			return (1);
		}
	}
	if (*buffer == PIPE)
	{
		ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_PIPE, ft_strndup(buffer, 1)));
		return (1);
	}
	if (*buffer == DOLLAR_SIGN)
	{
		int i = 1;
		end = buffer;
		while (*++end && *end != WHITESPACE
				&& *end != REDIRECT_LEFT && *end != PIPE
				&& *end != REDIRECT_RIGHT && *end != '$'
				&& *end != DOUBLE_QUOTES
				&& *end != SINGLE_QUOTES)
			i++;
		ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_DOLLAR, ft_strndup(buffer, end - buffer)));
		return (i);
	}
	if (*buffer == SINGLE_QUOTES)
	{
		ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_SINGLE_QUOTES, ft_strndup(buffer, 1)));
		return (1);
	}
	if (*buffer == DOUBLE_QUOTES)
	{
		ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_DOUBLE_QUOTES, ft_strndup(buffer, 1)));
		return (1);
	}
	return (0);
}

void	*token_reformatting(t_token **tokens)
{
	t_token		*head;
	t_token		*current;

	head = *tokens;
	current = *tokens;
	while (current && current->type != TOKEN_EOF)
	{
		if (current && current->type == TOKEN_EOF)
			return (NULL);
		if (current && current->type == TOKEN_PIPE)
		{
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				current->type = TOKEN_COMMAND;
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				current->type = TOKEN_APPENDICE;
			if (current && current->next)
				current = current->next;
			continue;
		}
		if (current && current->type != TOKEN_WORD
			&& current->type != TOKEN_OPTION
			&& current->type != TOKEN_DOLLAR)
		{
			current = current->next;
			if (current && current->type == 0)
				current->type = TOKEN_APPENDICE;
			current = current->next;
			if (current && current->type == 0)
				current->type = TOKEN_COMMAND;
			continue;
		}
		if (current && current->type == TOKEN_WORD)
		{
			current->type = TOKEN_COMMAND;
			current = current->next;
			while (current && (current->type == 0 && current->type != 7) || current->type == TOKEN_OPTION)
			{
				current->type = TOKEN_APPENDICE;
				current = current->next;
			}
			continue;
		}
		break;
	}
	return NULL;
}

