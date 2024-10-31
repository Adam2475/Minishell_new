/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 16:21:37 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_remove_ws(t_token **token)
{
	t_token	*tmp;

	tmp = (*token);
	while (tmp->type != TOKEN_EOF)
	{
		if (tmp->next->type == TOKEN_WHITESPACE)
			tkn_delone(&tmp, tmp->next);
		else
			tmp = tmp->next;
	}
}

void	free_node_env(t_env_list *node)
{
	t_env_list	*tmp;

	tmp = node;
	if (tmp->pre != NULL)
		tmp->pre->next = tmp->next;
	if (tmp->next != NULL)
		tmp->next->pre = tmp->pre;
	free(tmp->value);
	free(tmp->var);
	free(tmp->content);
	free(tmp);
	tmp = NULL;
}

int	ft_strsearch(char *str, int c)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static	void	clean_qt_helper(t_token **node)
{
	t_token	*new;

	new = NULL;
	tkn_delone(&(*node), (*node)->next);
	tkn_delone(&(*node), (*node)->next);
	new = (t_token *)ft_calloc(sizeof(t_token), 1);
	new->value = ft_strndup("", 2);
	new->type = 14;
	if (!(*node)->next)
		new->next = NULL;
	if ((*node)->next)
		new->next = (*node)->next;
	(*node)->next = new;
}

void	clean_qt(t_token **tkn)
{
	t_token	*node;

	node = *tkn;
	while (node && node->type != TOKEN_EOF)
	{
		if (node->next && node->next->next
			&& (node->next->type == 9 || node->next->type == 10)
			&& node->next->type == node->next->next->type)
		{
			clean_qt_helper(&node);
			continue ;
		}
		if (node->next->type == 9 || node->next->type == 10)
		{
			tkn_delone(&node, node->next);
			continue ;
		}
		if (node && node->type != TOKEN_EOF)
			node = node->next;
	}
}
