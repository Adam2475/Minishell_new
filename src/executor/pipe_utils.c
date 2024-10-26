/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:41:39 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/26 13:25:49 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	count_pipes(t_token *head)
{
	int			count;
	t_token		*current;

	count = 0;
	current = head;
	while (current != NULL)
	{
		if (current->type == TOKEN_PIPE)
		{
			count++;
		}
		current = current->next;
	}
	return (count);
}

void	close_pipes(int *end, int pipes)
{
	int	i;

	i = 0;
	while (i < (2 * pipes))
	{
		close(end[i]);
		i++;
	}
}

static	void	redirection_out_case_helper(t_data **data, int *end)
{
	ft_printf("no such file or directory!\n");
	free((*data)->parent);
	(*data)->parent = NULL;
	if ((*data)->in_tmp > 0)
		close((*data)->in_tmp);
	if ((*data)->fd_in > 0)
		close((*data)->fd_in);
	if ((*data)->fd > 0)
		close((*data)->fd);
	if ((*data)->fd_out > 0)
		close((*data)->fd_out);
	exec_exit3(data, &(*data)->tokens_ptr, end, 0);
}

static	void	redirection_in_case_helper(t_data **data, int *end)
{
	free((*data)->parent);
	heredoc_unlink(data);
	if ((*data)->in_tmp > 0)
		close((*data)->in_tmp);
	if ((*data)->fd_in > 0)
		close((*data)->fd_in);
	if ((*data)->fd > 0)
		close((*data)->fd);
	if ((*data)->fd_out > 0)
		close((*data)->fd_out);
	exec_exit3(data, &(*data)->tokens_ptr, end, 0);
}

void	setup_pipe(t_data **data, int i, int prev_fd, int *end)
{
	int	pipes;

	pipes = count_pipes((*data)->tokens);
	if (i > 0 && (*data)->hd_flag == 0)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (i < pipes)
		dup2(end[i * 2 + 1], STDOUT_FILENO);
	if ((*data)->redirect_state_out > 0)
	{
		if (dup2((*data)->fd_out, STDOUT_FILENO) < 0)
			redirection_out_case_helper(data, end);
		close((*data)->fd_out);
	}
	if ((*data)->redirect_state_in > 0)
	{
		dup2((*data)->fd_in, STDIN_FILENO);
		if ((*data)->fd_in < 0)
			redirection_in_case_helper(data, end);
		close((*data)->fd_in);
	}
}
