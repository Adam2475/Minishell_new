/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/15 15:21:06 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	process_command2(t_data **data, char **command)
{
	if ((*data)->merdoso == 0)
		(*data)->cmd2 = find_cmd(command[0], data);
	else if ((*data)->merdoso == 1)
	{
		(*data)->cmd2 = find_cmd(command[0], data);
		(*data)->merdoso = 0;
	}
}

static	int	redirect_builtin(t_data **data)
{
	if (!((*data)->fd < 0))
	{
		if ((*data)->redirect_state == 1)
		{
			(*data)->saved_fd = dup(STDOUT_FILENO);
			if (dup2((*data)->fd, STDOUT_FILENO) < 0)
				return (-1);
		}
		if ((*data)->redirect_state == 0)
		{
			(*data)->saved_fd = dup(STDIN_FILENO);
			if (dup2((*data)->fd, STDIN_FILENO) < 0)
				return (-1);
		}
	}
	return (0);
}

int	manual_cmd(char **cmd_args, t_data **data, t_token **token)
{
	t_data	*tmp;

	tmp = (*data);
	tmp->cmd = conf_man_cmd(cmd_args[0]);
	if (!tmp->cmd)
		return (0);
	if (redirect_builtin(data) < 0)
		return (-1);
	(*data)->cmd_args = NULL;
	clean_qt(token);
	if (tmp->cmd == CH_DIR)
		return (ft_remove_ws(token), cd_cmd(data, token), 1);
	if (tmp->cmd == ECHO)
		return (echo_cmd(token), 1);
	if (tmp->cmd == EXPORT)
		return (export_cmd(data, token), 1);
	if (tmp->cmd == UNSET)
		return (unset_env(token, &tmp->env_list), 1);
	if (tmp->cmd == ENV && !(*data)->merdoso)
		return (env_cmd(data, token), 1);
	if (tmp->cmd == EXIT)
		return (cmd_exit(data, token), 1);
	if (tmp->cmd == PWD)
		return (pwd_cmd(), 1);
	return (0);
}

int	conf_man_cmd(char *str)
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
