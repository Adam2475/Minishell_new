/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/30 11:58:13 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	expand_doll_2(t_env_list *node, t_token **current, char **tmp, int len)
{
	ft_free_null((*tmp));
	(*tmp) = ft_strtrim2((*current)->value, "$");
	(*current)->value = ft_strdup((*tmp));
	ft_free_null((*tmp));
	len = ft_isalpha_len2((*current)->value);
	if (len > 0)
	{
		(*tmp) = ft_strndup((*current)->value + len,
				ft_strlen((*current)->value) - len);
		ft_free_null((*current)->value);
		(*current)->value = ft_strjoin(node->value, (*tmp));
		return (ft_free_null((*tmp)), 0);
	}
	else
		ft_free_null((*current)->value);
	return ((*current)->value = ft_strndup(node->value,
			ft_strlen(node->value)), 0);
}

void	expand_doll_3(t_token **current, char **tmp, char **tmp2)
{
	if ((*current) && (*current)->value)
		(*tmp) = tmp_set((*current)->value);
	if ((*tmp) && ft_strlen((*tmp)) == 1
		&& (ft_isalpha((*tmp)[0]) || (*tmp)[0] == '_'))
	{
		(*tmp2) = (*tmp);
		(*tmp) = ft_strjoin((*tmp2), "=");
		ft_free_null((*tmp2));
	}
}

static	void	expand_doll4(t_env_list **node, char *tmp)
{
	while ((*node) && tmp && ft_strlen(tmp) > 1
		&& ft_strncmp(tmp, (*node)->var, ft_strlen(tmp) - 1) != 0)
	{
		if ((*node)->next)
			(*node) = (*node)->next;
		else if (!(*node)->next)
		{
			(*node) = (*node)->next;
			break ;
		}
	}
	while ((*node) && tmp && ft_strlen(tmp) <= 1
		&& ft_strncmp(tmp, (*node)->var, ft_strlen(tmp)) != 0)
	{
		if ((*node)->next)
			(*node) = (*node)->next;
		else if (!(*node)->next)
		{
			(*node) = (*node)->next;
			break ;
		}
	}
}

int	expand_doll(t_token **current, t_data **data)
{
	t_env_list	*node;
	char		*tmp;
	char		*tmp2;
	int			len;

	node = (*data)->env_list;
	len = 0;
	tmp = NULL;
	tmp2 = NULL;
	expand_doll_3(current, &tmp, &tmp2);
	expand_doll4(&node, tmp);
	if (!node || *tmp == '?')
		return (case_err(current, tmp, data));
	return (expand_doll_2(node, current, &tmp, len), 0);
}
