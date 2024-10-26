/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:00:51 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/26 19:59:41 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_token(t_token *token)
{
	if (token && token != NULL)
	{
		free(token->value);
		token->value = NULL;
		free(token);
		token = NULL;
	}
}

t_token_list	*create_token_list_node(t_token *token_head)
{
	t_token_list	*new_node;

	new_node = ft_calloc(sizeof(t_token_list), 1);
	if (!new_node)
		return (NULL);
	new_node->head = token_head;
	new_node->next = NULL;
	return (new_node);
}

t_token_list	*terminate_segment(t_token *prev)
{
	if (prev)
		prev->next = NULL;
	return (NULL);
}

t_token_list	*create_and_link(t_token *start, t_token_list *result,
					t_token_list *current_list)
{
	t_token_list	*new_list;

	new_list = create_token_list_node(start);
	if (!new_list)
		return (result);
	if (!result)
		return (new_list);
	current_list->next = new_list;
	return (new_list);
}

void	free_token_segment2(t_token *start)
{
	t_token	*tmp;
	t_token	*next_token;

	tmp = start;
	while (tmp)
	{
		next_token = tmp->next;
		free_token(tmp);
		tmp = next_token;
	}
}
