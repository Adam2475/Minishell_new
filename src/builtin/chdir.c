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
	free(null);
	null = NULL;
}

void	chpwd(t_data **data, char *new_path)
{
	t_env_list	*node;
	t_env_list	*node_old;

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
	node->value = ft_strndup(new_path, ft_strlen(new_path));
	node->content = ft_strjoin(node->var, node->value);
	return ;
}

int	cd_cmd(t_data **data, t_token **tkn)
{
	t_token		*current;
	t_env_list	*node;

	current = (*tkn)->next;
	ft_printf("%s && %s && %d\n", current->value, current->next->value, ft_lstsize_token((*tkn)));
	node = (*data)->env_list;
	if ((ft_lstsize_token((*tkn)) - 1) > 3
		&& (current->next->value[0] != '>'
		|| current->next->value[0] != '<'
		|| current->next->value[0] != '|'))
		return (g_err_state = 1,
		ft_printf("bash: cd: too many arguments\n"));
	node = (*data)->env_list;
	if (!current->value || current->value[0] == '~')
	{
		while (node && ft_strncmp(node->var, "HOME=", 5) != 0)
			node = node->next;
		ft_free_null(current->value);
		current->value = ft_strndup(node->value, ft_strlen(node->value));
	}
	if (chdir(current->value) != 0)
		return (ft_err_chdir((int)errno, current->value));
	chpwd(data, current->value);
	return (g_err_state = 0, 1);
}
