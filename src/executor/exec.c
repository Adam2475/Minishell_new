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

static	int	child_process(char **cmd_args, t_data **data, char **envp)
{
	if (!((*data)->fd < 0))
	{
		if ((*data)->redirect_state == 1)
		{
			if (dup2((*data)->fd, STDOUT_FILENO) < 0)
				return (-1);
		}
		if ((*data)->redirect_state == 0)
		{
			if (dup2((*data)->fd, STDIN_FILENO) < 0)
				return (-1);
		}
	}
	if ((*data)->cmd2 && cmd_args)
		execve((*data)->cmd2, cmd_args, envp);
	else
	{
		g_err_state = 127;
		exit(1);
	}
	return (EXIT_SUCCESS);
}

static	int	parent_process(void)
{
	int	status;

	waitpid(-1, &status, 0);
	return (status);
}

void	execute_command_single(char **command, t_data **data,
		char **envp, t_token **tokens)
{
	pid_t	parent;
	char	*tmp;
	char	*holder;
	int		i;

	init_execution(data, &i, command);
	tmp = ft_strjoin(command[0], " ");
	if (manual_cmd(command, data, tokens))
	{
		free(tmp);
		return ;
	}
	(*data)->cmd2 = find_cmd(command[0], data);
	holder = NULL;
	while (command[i])
	{
		holder = ft_strjoin_gnl(tmp, command[i++]);
		tmp = holder;
	}
	(*data)->cmd_args = ft_split(tmp, 32);
	free(tmp);
	parent = fork();
	if (parent < 0)
		free_exit(data, *tokens);
	if (!parent)
		child_process(command, data, envp);
	else
		g_err_state = parent_process();
	return ;
}
