/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/23 12:51:06 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	check_type(t_token *current)
{
	if (current && current->type != 0 && current->type != 1
		&& current->type != 11)
		return (1);
	else
		return (0);
}

static	int invalid_tkn_sequence(t_token_type prev_type, t_token_type curr_type)
{
	if (prev_type == TOKEN_PIPE && curr_type == TOKEN_PIPE)
	// Errore: due pipe consecutive
		return (1);
	if (prev_type == TOKEN_REDIRECT_OUT && (curr_type == TOKEN_PIPE
		|| curr_type == TOKEN_REDIRECT_OUT || curr_type == TOKEN_APPEND))
		// Errore: ridirezione seguita da un'altra ridirezione o pipe
		return (1);
	if (prev_type == TOKEN_REDIRECT_IN && curr_type == TOKEN_REDIRECT_IN)
		// Errore: doppia ridirezione in
		return (1);
	if (prev_type == TOKEN_HEREDOC && (curr_type == TOKEN_REDIRECT_IN
		|| curr_type == TOKEN_REDIRECT_OUT || curr_type == TOKEN_HEREDOC))
		// Errore: heredoc e redir in
		return (1);
	if (prev_type == TOKEN_APPEND && (curr_type == TOKEN_HEREDOC
		|| curr_type == TOKEN_REDIRECT_IN || curr_type == TOKEN_REDIRECT_OUT
		|| curr_type == TOKEN_APPEND))
		// Errore: heredoc e redir out
		return (1);
    // Aggiungere altre regole di sintassi qui
	return (0);
}

static	int check_syntax_errors(t_token *tokens)
{
    t_token *current = tokens;
    t_token *previous = NULL;

	while (current != NULL && current->type != 7)
	{
		if (previous == NULL && current->type == TOKEN_PIPE)
			return (1);
		if (previous != NULL && invalid_tkn_sequence(previous->type, current->type))
			return (1);
		if (current->next && current->type != 7 && current->type == 11)
		{
			current = current->next;
			continue ;
		}
		previous = current;
		current = current->next;
	}
	if (current && current->type == 7 && previous && previous->type == 2)
		return (1);
	return (0);
}

static	int	check_spaces(t_token *tokens)
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

int	token_reformatting(t_token **tokens)
{
	t_token		*head;
	t_token		*current;

	head = *tokens;
	current = *tokens;
	if (check_syntax_errors(*tokens) > 0)
		return (write(2, "syntax error\n", 14), g_err_state = 2, 1);
	if (check_spaces(*tokens))
		return (g_err_state = 0, 1);
	while (current && current->type != TOKEN_EOF)
	{
		while (current->type == TOKEN_WHITESPACE)
			current = current->next;
		while (current->type == TOKEN_DOUBLE_QUOTES)
			current = current->next;
		if ((current && current->type == TOKEN_EOF) || current == NULL)
			return (1);
		if (current && current->type == TOKEN_PIPE)
			current = token_reformatting_pipe(current);
		if (current->type == TOKEN_DOLLAR)
			current = current->next;
		if (check_type(current) > 0 && current->type != 2)
			current = token_reformatting_special(current);
		if (current && current->type == TOKEN_WORD)
			current = token_reformatting_command(current);
		else if (current && current->type != 2 && current->type != 6)
			current = current->next;
	}
	current = head;
	return (0);
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
