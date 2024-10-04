/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static	int	case_err(t_token **current, char *tmp)
{
	free((*current)->value);
	if (*tmp == '?')
	{
		(*current)->value = expand_err_state(tmp);
		return (0);
	}
	(*current)->value = ft_strndup("", 1);
	return (free(tmp), 0);
}

int	expand_doll(t_token **current, t_data **data)
{
	t_env_list	*node;
	char		*tmp;

	node = (*data)->env_list;
	tmp = tmp_set((*current)->value);
	while (node && ft_strncmp(tmp, node->var, ft_strlen(tmp) - 1) != 0)
	{
		if (node->next)
			node = node->next;
		else if (!node->next)
		{
			node = node->next;
			break ;
		}
	}
	if (!node)
		return (case_err(current, tmp));
	ft_free_null(tmp);
	free((*current)->value);
	return ((*current)->value = ft_strndup(node->value,
			ft_strlen(node->value)), 0);
}

int	expand_var(t_token **tkn_lst, t_data **data)
{
	t_token	*current;

	current = (*tkn_lst);
	while (current->type != TOKEN_EOF)
	{
		if (current->type == 8)
			expand_doll(&current, data);
		current = current->next;
	}
	current = (*tkn_lst);
	return (0);
}

static t_token	*ft_set_zero(t_token *current, int flag)
{
	while ((int)current->type != 7 && flag == 0
		&& (int)current->type != 10)
	{
		current->type = TOKEN_WORD_QT;
		current = current->next;
	}
	while ((int)current->type != 7 && (int)current->type != 9
		&& flag == 1)
	{
		if ((int)current->type != 8)
			current->type = TOKEN_WORD_QT;
		else if ((int)current->type == 8
			&& (int)current->next->type == 13)
			current = current->next;
		current = current->next;
	}
	return (current);
}

int	check_quotes(t_token **tokens)
{
	t_token	*current;

	current = (*tokens);
	while ((int)current->type != TOKEN_EOF)
	{
		if ((int)current->type == 10)
		{
			current = current->next;
			current = ft_set_zero(current, 0);
			if (current->type == TOKEN_EOF)
				return (ft_printf("unclosed quote\n"));
		}
		if ((int)current->type == 9)
		{
			current = current->next;
			current = ft_set_zero(current, 1);
			if (current->type == TOKEN_EOF)
				return (ft_printf("unclosed quote\n"));
		}
		current = current->next;
	}
	return (0);
}
