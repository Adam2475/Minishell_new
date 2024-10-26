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

static	void	child_process(char **cmd_args, t_data **data,
	t_token **tokens)
{
	if ((*data)->redirect_state_out > 0)
	{
		(*data)->saved_fd = dup(STDOUT_FILENO);
		if (dup2((*data)->fd_out, STDOUT_FILENO) < 0)
<<<<<<< HEAD
		{
			write(2, "Permission denied\n", 19);
			exec_exit(data, tokens, 1);
		}
=======
			exec_exit(data, tokens, 2);
>>>>>>> 1c252a35d925e6972e81f9e762829b1851e9110c
	}
	if ((*data)->redirect_state_in > 0)
	{
		if (dup2((*data)->fd_in, STDIN_FILENO) < 0)
<<<<<<< HEAD
		{
			write(2, "Permission denied\n", 19);
			exec_exit(data, tokens, 1);
		}
=======
			exec_exit(data, tokens, 2);
>>>>>>> 1c252a35d925e6972e81f9e762829b1851e9110c
	}
	if ((*data)->cmd2 && cmd_args && copy_mtx1(data))
	{
		(*data)->cmd2 = trim_quotes((*data)->cmd2);
		errno = errno % 256;
		if (execve((*data)->cmd2, cmd_args, (*data)->env_p) != 0)
			exec_exit(data, tokens, 126);
	}
	exec_exit(data, tokens, 127);
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
	int		i;

	(*data)->saved_fd = -1;
	init_execution(data, &i);
	(*data)->tmp9 = ft_strjoin(command[0], " ");
	if (manual_cmd(command, data, tokens))
		return (errno = g_err_state, command_single_helper(data),
			free((*data)->tmp9));
	process_command2(data, command);
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
