/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/07 12:26:08 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_err_state(char *tmp)
{
	char	*err;
	char	*tmp2;

	err = ft_itoa(g_err_state);
	tmp2 = ft_strtrim2(tmp, "=");
	tmp = ft_strtrim2(tmp2, "?");
	tmp2 = ft_strndup(tmp, ft_strlen(tmp));
	free(tmp);
	tmp = ft_strjoin(err, tmp2);
	free(tmp2);
	free(err);
	return (tmp);
}

char	*tmp_set(char *val)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strndup(val, ft_strlen(val));
	tmp2 = ft_strtrim2(tmp, "$");
	tmp = ft_strjoin(tmp2, "=");
	ft_free_null(tmp2);
	return (tmp);
}

void	process_command2(t_data **data, char **command)
{
	if (!(*data)->merdoso)
		(*data)->cmd2 = find_cmd(command[0], data);
	else
		(*data)->cmd2 = ft_strndup((*data)->input, ft_strlen((*data)->input));
}

int	manual_cmd(char **cmd_args, t_data **data, t_token **token)
{
	t_data	*tmp;

	tmp = (*data);
	tmp->cmd = conf_man_cmd(cmd_args[0]);
	(*data)->cmd_args = NULL;
	clean_qt(token);
	if (tmp->cmd == CH_DIR)
		return (ft_remove_ws(token), cd_cmd(data, token));
	if (tmp->cmd == ECHO)
		return (echo_cmd(token));
	if (tmp->cmd == EXPORT)
		return (export_cmd(data, token));
	if (tmp->cmd == UNSET)
		return (unset_env(token, &tmp->env_list));
	if (tmp->cmd == ENV)
		return (env_cmd(data));
	if (tmp->cmd == EXIT)
		return (cmd_exit(data, token));
	if (tmp->cmd == PWD)
		return (pwd_cmd(data));
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
