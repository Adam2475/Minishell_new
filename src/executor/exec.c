/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 19:21:15 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	copy_mtx1_pt2(t_data **data, int i)
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

static	int	copy_mtx1(t_data **data)
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

static	int	child_process(char **cmd_args, t_data **data,
	t_token **tokens)
{
	if (!((*data)->fd < 0))
	{
		if ((*data)->redirect_state == 1)
		{
			(*data)->saved_fd = dup(STDOUT_FILENO);
			if (dup2((*data)->fd, STDOUT_FILENO) < 0)
				exit(2);
		}
		if ((*data)->redirect_state == 0)
		{
			if (dup2((*data)->fd, STDIN_FILENO) < 0)
				exit(2);
		}
	}
	if ((*data)->cmd2 && cmd_args && copy_mtx1(data))
	{
		(*data)->cmd2 = trim_quotes((*data)->cmd2);
		errno = errno % 256;
		copy_mtx1(data);
		if (execve((*data)->cmd2, cmd_args, (*data)->env_p) != 0)
			exec_exit(data, tokens, 126);
	}
	else
		exec_exit(data, tokens, 127);
	exit(EXIT_FAILURE);
}

static	int	parent_process(void)
{
	int	status;

	waitpid(-1, &status, 0);
	return (status);
}

void	execute_command_single(char **command, t_data **data,
		t_token **tokens)
{
	pid_t	parent;
	char	*holder;
	int		i;

	(*data)->saved_fd = -1;
	init_execution(data, &i);
	(*data)->tmp9 = ft_strjoin(command[0], " ");
	if (manual_cmd(command, data, tokens))
		return (errno = g_err_state, command_single_helper(data),
			free((*data)->tmp9));
	process_command2(data, command);
	holder = NULL;
	holder = command_single_finder(&i, data, command);
	(*data)->cmd_args = ft_split((*data)->tmp9, 32);
	free((*data)->tmp9);
	parent = fork();
	if (parent < 0)
		free_exit(data);
	if (!parent)
		child_process(command, data, tokens);
	else
		g_err_state = parent_process();
	return ;
}
