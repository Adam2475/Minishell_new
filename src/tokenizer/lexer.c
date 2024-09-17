/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	token_reformatting(t_token **tokens)
{
	t_token		*head;
	t_token		*current;

	head = *tokens;
	current = *tokens;
	while (current && current->type != TOKEN_EOF)
	{
		if (current && current->type == TOKEN_EOF)
			return ;
		if (current && current->type == TOKEN_PIPE)
		{
			while (current->type == TOKEN_WHITESPACE)
				current = current->next;
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				current->type = TOKEN_COMMAND;
			while (current->type == TOKEN_WHITESPACE)
				current = current->next;
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				current->type = TOKEN_APPENDICE;
			if (current && current->next)
				current = current->next;
			continue ;
		}
		if (current && current->type != TOKEN_WORD
			&& current->type != TOKEN_OPTION
			&& current->type != TOKEN_DOLLAR)
		{
			while (current->type == TOKEN_WHITESPACE)
				current = current->next;
			current = current->next;
			if (current && current->type == 0)
				current->type = TOKEN_APPENDICE;
			while (current->type == TOKEN_WHITESPACE)
				current = current->next;
			current = current->next;
			if (current && current->type == 0)
				current->type = TOKEN_COMMAND;
			continue ;
		}
		if (current && current->type == TOKEN_WORD)
		{
			current->type = TOKEN_COMMAND;
			while (current->type == TOKEN_WHITESPACE)
				current = current->next;
			while (current && ((current->type == 0 && current->type != 7)
					|| current->type == TOKEN_OPTION))
			{
				current->type = TOKEN_APPENDICE;
				while (current->type == TOKEN_WHITESPACE)
					current = current->next;
				current = current->next;
			}
			continue ;
		}
		break ;
	}
	current = head;
}

static	void	recognizer(char *buffer, t_token **tokens,
		char *end, t_data **data)
{
	while (*buffer)
	{
		end = buffer;
		if (*buffer == WHITESPACE)
		{
			buffer = buffer + whitespace_case(buffer, end, tokens);
			continue ;
		}
		if (*buffer == REDIRECT_LEFT || *buffer == REDIRECT_RIGHT
			|| *buffer == PIPE || *buffer == '$' || *buffer == SINGLE_QUOTES
			|| *buffer == DOUBLE_QUOTES)
		{
			buffer = buffer + special_cases_lexer(data, buffer, tokens, end);
			continue ;
		}
		end = buffer;
		while (*end && *end != WHITESPACE && *end != REDIRECT_LEFT
			&& *end != PIPE && *end != REDIRECT_RIGHT && *end != '$'
			&& *end != DOUBLE_QUOTES && *end != SINGLE_QUOTES)
			end++;
		if (*buffer == '-')
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_OPTION,
					ft_strndup(buffer, end - buffer)));
		else
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_WORD,
					ft_strndup(buffer, end - buffer)));
		buffer = end;
	}
}

static	int	init_state(t_data **data, t_token **tokens, char *tmp)
{
	if (data)
		*tokens = NULL;
	if (!tmp)
		return (1);
	return (0);
}

void	tokenizer(t_data **data, t_token **tokens)
{
	char	*buffer;
	char	*tmp;
	char	*end;

	end = NULL;
	tmp = ft_strndup((*data)->input, ft_strlen((*data)->input));
	if (init_state(data, tokens, tmp) > 0)
	{
		free(tmp);
		free_exit(data);
	}
	buffer = tmp;
	end = buffer;
	recognizer(buffer, tokens, end, data);
	token_reformatting(tokens);
	free (tmp);
}
