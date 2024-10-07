/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/07 12:28:18 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
// TODO: sistema

void	append_token_list(t_token_list **list, t_token *head)
{
	t_token_list	*new_node;
	t_token_list	*current;

	new_node = create_token_list_node(head);
	if (!new_node)
		return ;
	if (*list == NULL)
		*list = new_node;
	else
	{
		current = *list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

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
	if (!current || current->type == TOKEN_EOF)
		return (current);
	current = current->next;
	while (current && (current->type == 11 || current->type == 9
			|| current->type == 10))
		current = current->next;
	while (current && current->type == 0)
		current->type = TOKEN_APPENDICE;
	if (!current || current->type == TOKEN_EOF)
		return (current);
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

void	parent_process2(t_data **data, int i, int *end, int parent)
{
	if (i > 0)
		close((*data)->prev_fd);
	if (i < (*data)->pipes)
	{
		close(end[i * 2 + 1]);
		(*data)->prev_fd = end[i * 2];
	}
	waitpid(parent, NULL, 0);
}
