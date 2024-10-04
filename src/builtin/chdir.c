/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/19 17:44:08 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	ft_err_chdir(int err, char *val)
{
	if (err == ENOENT)
		return (g_err_state = err,
			ft_printf("bash: cd: %s: No such file or directory\n", val));
	else if (err == ENOTDIR)
		return (g_err_state = err,
			ft_printf("bash: cd: %s: Not a directory\n", val));
	return (0);
}

void	ft_free_null(void *null)
{
	char	*tmp;

	tmp = (char *)null;
	if (null && tmp && tmp[0])
		free(null);
	null = NULL;
	tmp = NULL;
}

static	void	chpwd(t_data **data)
{
	t_env_list	*node;
	t_env_list	*node_old;
	char		*tmp;

	node = (*data)->env_list;
	while (node && ft_strncmp(node->var, "PWD=", 4) != 0)
		node = node->next;
	node_old = (*data)->env_list;
	while (node_old && ft_strncmp(node_old->var, "OLDPWD=", 7) != 0)
		node_old = node_old->next;
	if (node == NULL || node_old == NULL)
		return ;
	ft_free_null(node_old->value);
	ft_free_null(node_old->content);
	node_old->value = ft_strndup(node->value, ft_strlen(node->value));
	node_old->content = ft_strjoin(node_old->var, node->value);
	ft_free_null(node->value);
	ft_free_null(node->content);
	tmp = getcwd(NULL, 0);
	node->value = ft_strndup(tmp, ft_strlen(tmp));
	node->content = ft_strjoin(node->var, node->value);
	free(tmp);
	return ;
}

int	cd_cmd(t_data **data, t_token **tkn)
{
	t_token		*current;
	t_env_list	*node;

	current = (*tkn)->next;
	node = (*data)->env_list;
	if ((ft_lstsize_token((*tkn)) - 1) > 3
		&& (current->next->value[0] != '>'
			|| current->next->value[0] != '<'
			|| current->next->value[0] != '|'))
		return (g_err_state = 1,
			ft_printf("bash: cd: too many arguments\n"));
	node = (*data)->env_list;
	if (current->value[0] == '\0')
	{
		while (node && ft_strncmp(node->var, "HOME=", 5) != 0)
			node = node->next;
		ft_free_null(current->value);
		current->value = ft_strndup(node->value, ft_strlen(node->value));
	}
	if (chdir(current->value) != 0)
		return (ft_err_chdir((int)errno, current->value));
	chpwd(data);
	return (g_err_state = 0, 1);
}
