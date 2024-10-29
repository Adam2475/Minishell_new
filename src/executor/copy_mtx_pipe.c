/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_mtx_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 22:19:59 by mapichec          #+#    #+#             */
/*   Updated: 2024/10/29 13:15:43 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	copy_mtx1_pt2(t_data **data, int i)
{
	t_env_list	*node;
	int			j;

	i = 0;
	node = (*data)->env_list;
	while (node)
	{
		if (i++ && !node->next)
			break ;
		else
			node = node->next;
	}
	(*data)->env_p = ft_calloc(sizeof(char *), i + 1);
	if (!(*data)->env_p)
		return (0);
	node = (*data)->env_list;
	j = 0;
	while (node && j <= i)
	{
		(*data)->env_p[j] = ft_strndup(node->content, ft_strlen(node->content));
		node = node->next;
		j++;
	}
	return (1);
}

int	copy_mtx1(t_data **data)
{
	t_env_list	*node;
	int			i;

	i = 0;
	node = (*data)->env_list;
	while (node)
	{
		if (!node->next)
		{
			i++;
			break ;
		}
		else
		{
			i++;
			node = node->next;
		}
	}
	if (!copy_mtx1_pt2(data, i))
		return (0);
	return (1);
}

int	copy_mtx2_pt2(t_data **data, int i)
{
	t_env_list	*node;
	int			j;

	(*data)->env_p = ft_calloc(sizeof(char *), i + 1);
	if (!(*data)->env_p)
		return (1);
	node = (*data)->env_list;
	j = 0;
	while (node && j < i)
	{
		(*data)->env_p[j] = ft_strndup(node->content, ft_strlen(node->content));
		node = node->next;
		j++;
	}
	return (0);
}

void	copy_mtx2(t_data **data)
{
	t_env_list	*node;
	int			i;

	i = 0;
	node = (*data)->env_list;
	while (node)
	{
		if (!node->next)
		{
			i++;
			break ;
		}
		else
		{
			i++;
			node = node->next;
		}
	}
	if (copy_mtx2_pt2(data, i))
		perror("");
}
