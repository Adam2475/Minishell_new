/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:03:48 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/24 14:30:52 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*command_single_finder(int *i,
	t_data **data, char **command)
{
	char	*tmp;

	tmp = NULL;
	while (command[(*i)])
	{
		tmp = ft_strjoin_gnl((*data)->tmp9, command[(*i)++]);
		(*data)->tmp9 = tmp;
	}
	return (tmp);
}

void	pipe_helper(t_data **data, t_token_list *current, int i)
{
	setup_pipe(data, i, (*data)->prev_fd, (*data)->end);
	close_pipes((*data)->end, (*data)->pipes);
	ft_tokenadd_back(&current->head, ft_lstnewtoken(7, NULL));
}

void	pipe_opener(t_data **data, int *end)
{
	int	j;

	j = 0;
	while (j < (*data)->pipes)
	{
		pipe(end + (j * 2));
		j++;
	}
}

void	init_pipe(t_data **data, t_token **tokens, int *i, int *flag)
{
	*flag = 0;
	(*data)->prev_fd = 0;
	(*data)->pipes = count_pipes(*tokens);
	*i = -1;
	(*data)->end = ft_calloc(sizeof(int), (*data)->pipes * 2);
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
