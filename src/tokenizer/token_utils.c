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

t_token *copy_token(t_token *token)
{
	if (!token)
		return (NULL);

	t_token *new_token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!new_token)
		return (NULL);

	new_token->type = token->type;
	if (token->value)
		new_token->value = ft_strndup(token->value, ft_strlen(token->value));
	else
		new_token->value = NULL;
	new_token->next = NULL;

	return (new_token);
}

t_token	*token_reformatting_special(t_token *current)
{
	current = current->next;
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current && current->type == 0)
		current->type = TOKEN_APPENDICE;
	current = current->next;
	return (current);
}

t_token	*token_reformatting_pipe(t_token *current)
{
	current = current->next;
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current && current->type == TOKEN_WORD)
		current->type = TOKEN_COMMAND;
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	current = current->next;
	if (current && current->type == TOKEN_WORD)
	{
		current->type = TOKEN_APPENDICE;
		if (current && current->next)
			current = current->next;
	}
	return (current);
}

t_token *copy_token_list(t_token *tokens)
{
	if (!tokens)
		return NULL;

	int i = 0;
	t_token *new_list = NULL;
	t_token *last_copied = NULL;
	t_token *current = tokens;
	int total = ft_lstsize_token(tokens);

	while (current && i < (total))
	{
		t_token *new_token = copy_token(current);

		if (!new_list)
			new_list = new_token;
		else
			last_copied->next = new_token;

		last_copied = new_token;
		current = current->next;
		i++;
	}
	return (new_list);
}