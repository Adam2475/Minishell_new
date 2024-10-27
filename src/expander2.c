/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/27 14:45:13 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	case_err(t_token **current, char *tmp, t_data **data)
{
	free((*current)->value);
	if (*tmp == '?')
	{
		(*current)->value = expand_err_state(tmp, data);
		return (0);
	}
	(*current)->value = ft_strndup("", 1);
	return (free(tmp), 0);
}

int	ft_isalpha_len2(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if ((str[i] <= 122 && str[i] >= 97) || (str[i] <= 90 && str[i] >= 65))
			i++;
		else if (str[i] == '_')
			i++;
		else if (str[i] != '\0')
			return (i);
		else
			return (-1);
	}
	return (-1);
}

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
	if ((*current)->value)
		(*tmp) = tmp_set((*current)->value);
	if ((*tmp) && ft_strlen((*tmp)) == 1
		&& (ft_isalpha((*tmp)[0]) || (*tmp)[0] == '_'))
	{
		(*tmp2) = (*tmp);
		(*tmp) = ft_strjoin((*tmp2), "=");
		ft_free_null((*tmp2));
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
	while (node && tmp && ft_strlen(tmp) > 1
		&& ft_strncmp(tmp, node->var, ft_strlen(tmp) - 1) != 0)
	{
		if (node->next)
			node = node->next;
		else if (!node->next)
		{
			node = node->next;
			break ;
		}
	}
	while (node && tmp && ft_strlen(tmp) <= 1
		&& ft_strncmp(tmp, node->var, ft_strlen(tmp)) != 0)
	{
		if (node->next)
			node = node->next;
		else if (!node->next)
		{
			node = node->next;
			break ;
		}
	}
	if (!node || *tmp == '?')
		return (case_err(current, tmp, data));
	return (expand_doll_2(node, current, &tmp, len), 0);
}
