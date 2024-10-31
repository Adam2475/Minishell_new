/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 16:38:16 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_type(t_token *current)
{
	if (current && current->type != 0 && current->type != 1
		&& current->type != 11)
		return (1);
	else
		return (0);
}

static	int	invalid_tkn_sequence(t_token_type prev_type, t_token_type curr_type)
{
	if (prev_type == 2 && curr_type == 2)
		return (1);
	if (prev_type == 4 && (curr_type == 2
			|| curr_type == 4 || curr_type == 5
			|| curr_type == 7))
		return (1);
	if (prev_type == 3 && (curr_type == 3 || curr_type == 7
			|| curr_type == 4))
		return (1);
	if (prev_type == 6 && (curr_type == 3
			|| curr_type == 4 || curr_type == 6
			|| curr_type == 7))
		return (1);
	if (prev_type == 5 && (curr_type == 6
			|| curr_type == 3 || curr_type == 4
			|| curr_type == 5 || curr_type == 7))
		return (1);
	return (0);
}

static	void	check_syntax_err2(t_token **current, t_token **previous)
{
	t_token_type	type;

	type = 0;
	type = (*current)->type;
	(*current) = (*current)->next;
	while ((*current) && (*current)->type != type && (*current)->type != 7)
		(*current) = (*current)->next;
	*previous = *current;
	if ((*current) && (*current)->type == type)
		(*current) = (*current)->next;
}

static	int	check_syntax_errors(t_token *tokens)
{
	t_token	*current;
	t_token	*previous;

	current = tokens;
	previous = NULL;
	while (current != NULL)
	{
		if (current->type == 9 || current->type == 10)
			check_syntax_err2(&current, &previous);
		if (previous == NULL && current->type == 2)
			return (1);
		if (previous != NULL
			&& invalid_tkn_sequence(previous->type, current->type))
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

int	token_reformatting(t_token **tokens, t_data **data)
{
	t_token		*head;
	t_token		*current;

	head = *tokens;
	current = *tokens;
	if (check_syntax_errors(*tokens) > 0)
		return (write(2, "syntax error\n", 14),
			(*data)->local_err_state = 2, 1);
	if (check_spaces(*tokens))
		return ((*data)->local_err_state = 0, 1);
	while (current && current->type != TOKEN_EOF)
		token_reformatting_helper(&current);
	if (check_token_type(*tokens) < 0)
		return ((*data)->local_err_state = 127, 1);
	return (current = head, 0);
}
