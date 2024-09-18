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
	if ((*data)->cmd && cmd_args)
		execve((*data)->cmd, cmd_args, envp);
	else
		exit(0);
	return (EXIT_SUCCESS);
}

static	int	conf_man_cmd(char *str)
{
	if (ft_strncmp(str, "cd", ft_strlen(str)) == 0)
		return (1);
	if (ft_strncmp(str, "echo", ft_strlen(str)) == 0)
		return (2);
	if (ft_strncmp(str, "export", ft_strlen(str)) == 0)
		return (3);
	if (ft_strncmp(str, "unset", ft_strlen(str)) == 0)
		return (4);
	if (ft_strncmp(str, "env", ft_strlen(str)) == 0)
		return (5);
	if (ft_strncmp(str, "exit", ft_strlen(str)) == 0)
		return (6);
	if (ft_strncmp(str, "pwd", ft_strlen(str)) == 0)
		return (7);
	else
		return (0);
}

static	int	parent_process(void)
{
	int	status;

	waitpid(-1, &status, 0);
	return (status);
}

static	int	manual_cmd(char **cmd_args, t_data **data)
{
	int		i;
	t_data	*tmp;

	tmp = (*data);
	i = 0;
	tmp->cmd = conf_man_cmd(cmd_args[0]);
	if (tmp->cmd == CH_DIR)
		return (cd_cmd(cmd_args, data));
	if (tmp->cmd == ECHO)
		return (echo_cmd(data, &tmp->tokens));
	if (tmp->cmd == EXPORT)
		return (export_cmd(cmd_args, data));
	if (tmp->cmd == UNSET)
		return (unset_env(&tmp->env_list, cmd_args[1]));
	if (tmp->cmd == ENV)
		return (env_cmd(data));
	if (tmp->cmd == EXIT)
		cmd_exit(cmd_args, *data);
	if (tmp->cmd == PWD)
		return (pwd_cmd(data));
	return (0);
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
	if (manual_cmd(command, data))
		return (free(tmp));
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
		free_exit((*data)->cmd_args, *tokens);
	if (!parent)
		child_process(command, data, envp);
	else
		g_err_state = parent_process();
	return ;
}
