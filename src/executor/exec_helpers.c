/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:03:48 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 16:15:11 by adapassa         ###   ########.fr       */
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

void	pipe_helper(t_data **data, t_token_list *current,
	pid_t *parent, t_token **tokens)
{
	if ((*data)->skip_flag)
		exec_exit3(data, tokens, (*data)->end, 0);
	setup_pipe(data, (*data)->counter, (*data)->prev_fd, (*data)->end);
	close_pipes((*data)->end, (*data)->pipes);
	ft_tokenadd_back(&current->head, ft_lstnewtoken(7, NULL));
	child_process_pipe(data, current->head, tokens, parent);
}

void	pipe_opener(t_data **data, int *end, int *flag)
{
	int	j;

	j = 0;
	while (j < (*data)->pipes)
	{
		pipe(end + (j * 2));
		j++;
	}
	*flag = 0;
}

void	init_pipe(t_data **data, t_token **tokens, pid_t *parent)
{
	(*data)->status = 0;
	(*data)->in_tmp = 0;
	(*data)->hd_flag = 0;
	(*data)->prev_fd = 0;
	(*data)->pipes = count_pipes(*tokens);
	(*data)->counter = -1;
	(*data)->end = ft_calloc(sizeof(int), (*data)->pipes * 2);
	(*data)->in_tmp = dup(STDIN_FILENO);
	(*data)->skip_flag = 0;
	(*data)->tokens_ptr = (*tokens);
	(*data)->parent = parent;
}
