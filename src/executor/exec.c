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
			exec_exit(data, tokens, 1);
	}
	if ((*data)->redirect_state_in > 0)
	{
		if (dup2((*data)->fd_in, STDIN_FILENO) < 0)
			exec_exit(data, tokens, 1);
	}
	if ((*data)->cmd2 && cmd_args && copy_mtx1(data))
	{
		(*data)->cmd2 = trim_quotes((*data)->cmd2);
		(*data)->local_err_state = (*data)->local_err_state % 256;
		signal(SIGQUIT, SIG_DFL);
		if (execve((*data)->cmd2, cmd_args, (*data)->env_p) != 0)
			exec_exit(data, tokens, 126);
	}
	exec_exit(data, tokens, 127);
}

static	void	parent_process(t_data **data)
{
	int	status;

	status = 0;
	waitpid(-1, &status, 0);
	if (WEXITSTATUS(status))
	{
		(*data)->local_err_state = status;
		if ((*data)->local_err_state < 0 || (*data)->local_err_state >= 255)
			(*data)->local_err_state = (*data)->local_err_state % 255;
	}
	else if (WIFSIGNALED(status))
	{
		if (g_err_state == 130 || status == 2)
			(*data)->local_err_state = 130;
		if (g_err_state == 131 || status == 131)
			(*data)->local_err_state = 131;
	}
	else
		(*data)->local_err_state = status;
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
		return (command_single_helper(data),
			free((*data)->tmp9));
	process_command2(data, command);
	(*data)->cmd_args = ft_split((*data)->tmp9, 32);
	free((*data)->tmp9);
	(*data)->tmp9 = NULL;
	parent = fork();
	if (parent < 0)
		free_exit(data);
	if (!parent)
		child_process(command, data, tokens);
	else
		parent_process(data);
	return ;
}
