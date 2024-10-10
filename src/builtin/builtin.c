/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/07 12:27:37 by adapassa         ###   ########.fr       */
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

// static	t_token	*join_in_qt_cm(t_token *tkn)
// {
// 	t_token			*current;
// 	t_token_type	type;
// 	char			*tmp;

// 	current = tkn;
// 	while (current && current->type == 11)
// 		current = current->next;
// 	if (current->type != 9 && current->type != 10)
// 		return (tkn);
// 	type = current->type;
// 	current = current->next;
// 	while (current->next && current->next->type != type)
// 	{
// 		tmp = current->value;
// 		current->value = ft_strjoin(current->value, current->next->value);
// 		free(tmp);
// 		tkn_delone(&current, current->next);
// 	}
// 	if (current->type == 12 || current->type == 9
// 		|| current->type == 10 || current->type == 14)
// 		current = current->next;
// 	return (current);
// }

void	clean_qt(t_token **tkn)
{
	t_token	*node;

	node = *tkn;
	// while (node && node->type != TOKEN_EOF)
	// {
	// 	if (node->type == 9 || node->type == 10)
	// 		node = join_in_qt_cm(node);
	// 	if (node && node->type != TOKEN_EOF)
	// 		node = node->next;
	// }
	// node = *tkn;
	while (node && node->type != TOKEN_EOF)
	{
		if (node->next->type == 9 || node->next->type == 10)
			tkn_delone(&node, node->next);
		if (node && node->type != TOKEN_EOF)
			node = node->next;
	}
}
