/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/04 17:11:50 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	token_reformatting(t_token **tokens)
{
	t_token		*head;
	t_token		*current;

	head = *tokens;
	current = *tokens;
	while (current && current->type != TOKEN_EOF)
	{
		while (current->type == TOKEN_WHITESPACE)
			current = current->next;
		while (current->type == TOKEN_DOUBLE_QUOTES)
			current = current->next;
		if ((current && current->type == TOKEN_EOF) || current == NULL)
			return ;
		if (current && current->type == TOKEN_PIPE)
			current = token_reformatting_pipe(current);
		if (current->type == TOKEN_DOLLAR)
			current = current->next;
		if (current && current->type != TOKEN_WORD
			&& current->type != TOKEN_OPTION
			&& current->type != 11)
			current = token_reformatting_special(current);
		if (current && current->type == TOKEN_WORD)
			current = token_reformatting_command(current);
		else if (current && current->type != TOKEN_PIPE)
			current = current->next;
	}
	current = head;
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

void	recognizer(char *buffer, t_token **tokens,
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
}

int	init_state(t_data **data, t_token **tokens, char *tmp)
{
	if (data)
		*tokens = NULL;
	if (!tmp)
		return (1);
	return (0);
}
