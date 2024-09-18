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

t_token	*copy_token(t_token *token)
{
	t_token	*new_token;

	if (!token)
		return (NULL);
	new_token = (t_token *)ft_calloc(sizeof(t_token), 1);
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

t_token	*copy_token_list(t_data **data, t_token *tokens)
{
	t_token		*new_list;
	t_token		*last_copied;
	t_token		*current;
	int			i;

	if (!tokens)
		return (NULL);
	i = 0;
	new_list = NULL;
	last_copied = NULL;
	current = tokens;
	(*data)->total = ft_lstsize_token(tokens);
	while (current && (i < (*data)->total))
	{
		(*data)->new_token = copy_token(current);
		if (!new_list)
			new_list = (*data)->new_token;
		else
			last_copied->next = (*data)->new_token;
		last_copied = (*data)->new_token;
		current = current->next;
		i++;
	}
	return (new_list);
}
