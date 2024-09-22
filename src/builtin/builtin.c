/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/22 19:26:45 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

static void	ft_remove_ws(t_token **token)
{
	t_token	*tmp;

	tmp = (*token);
	while (tmp->type != TOKEN_EOF)
	{
		if (tmp->next->type == TOKEN_WHITESPACE)
			tkn_delone(&tmp, tmp->next);
		else
			tmp = tmp->next;
	}
}

void	free_node_env(t_env_list *node)
{
	t_env_list	*tmp;

	tmp = node;
	node->pre->next = node->next;
	free(tmp->value);
	free(tmp->var);
	free(tmp->content);
	free(tmp);
	tmp = NULL;
}

int	manual_cmd(char **cmd_args, t_data **data, t_token **token)
{
	t_data	*tmp;

	tmp = (*data);
	tmp->cmd = conf_man_cmd(cmd_args[0]);
	(*data)->cmd_args = NULL;
	if (tmp->cmd == CH_DIR)
		return (ft_remove_ws(token), cd_cmd(data, token));
	if (tmp->cmd == ECHO)
		return (echo_cmd(token));
	if (tmp->cmd == EXPORT)
		return (export_cmd(data, token));
	if (tmp->cmd == UNSET)
		return (unset_env(&tmp->env_list, cmd_args[1]));
	if (tmp->cmd == ENV)
		return (env_cmd(data));
	if (tmp->cmd == EXIT)
		return (cmd_exit(data));
	if (tmp->cmd == PWD)
		return (pwd_cmd(data));
	return (0);
}
