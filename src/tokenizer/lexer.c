/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/24 19:29:15 by adapassa         ###   ########.fr       */
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
		while (current->type == TOKEN_WHITESPACE)
			current = current->next;
		if ((current && current->type == TOKEN_EOF) || current == NULL)
			return ;
		if (current && current->type == TOKEN_PIPE)
			current = token_reformatting_pipe(current);
		if (current && current->type != TOKEN_WORD
			&& current->type != TOKEN_OPTION && current->type != TOKEN_DOLLAR)
			current = token_reformatting_special(current);
		if (current && current->type == TOKEN_WORD)
			current = token_reformatting_command(current);
		else if (current && current->type != TOKEN_PIPE)
			current = current->next;
	}
	current = head;
}

static	int	find_special(char c)
{
	if (c && c != WHITESPACE && c != REDIRECT_LEFT
		&& c != PIPE && c != REDIRECT_RIGHT && c != '$'
		&& c != DOUBLE_QUOTES && c != SINGLE_QUOTES)
		return (0);
	else
		return (1);
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
		while (*end && find_special(*end) < 1)
			end++;
		if (*buffer == '-')
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_OPTION,
					ft_strndup(buffer, end - buffer)));
		else
			ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_WORD,
					ft_strndup(buffer, end - buffer)));
		buffer = end;
	}
	ft_tokenadd_back(tokens, ft_lstnewtoken(TOKEN_EOF,
			ft_strndup(buffer, *buffer)));
}

static	int	init_state(t_data **data, t_token **tokens, char *tmp)
{
	if (data)
		*tokens = NULL;
	if (!tmp)
		return (1);
	return (0);
}

int	tokenizer(t_data **data, t_token **tokens)
{
	char	*buffer;
	char	*tmp;
	char	*end;

	end = NULL;
	if (!(*data)->input)
		free_exit(data, *tokens);
	tmp = ft_strndup((*data)->input, ft_strlen((*data)->input));
	if (init_state(data, tokens, tmp) > 0)
	{
		free(tmp);
		free_exit(data, *tokens);
	}
	buffer = tmp;
	end = buffer;
	recognizer(buffer, tokens, end, data);
	token_reformatting(tokens);
	if (check_quotes(tokens) != 0)
		return (1);
	expand_var(tokens, data);
	free (tmp);
	return (0);
}
