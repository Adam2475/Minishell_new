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

static	int	check_err_tkn(t_token **tkn)
{
	t_token	*node;

	node = *tkn;
	while (node->type != 7 && (node->type == 11 || node->type == 12))
		node = node->next;
	if (node && node->type != 7 && node->next)
		node = node->next;
	while (node->type != 3 && node->type != 4
			&& node->type != 2 && node->type != 6
			&& node->type != 7 && node)
	{
		if (node->type != 11)
			return (1);
		if (node->type == 11)
			node = node->next;
		if (!node || node->type == 7)
			return (0);
	}
	return (0);
}

static	int	cd_null(t_token *current, t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	while (node && ft_strncmp(node->var, "HOME=", 5) != 0)
		node = node->next;
	ft_free_null(current->value);
	if (node)
		current->value = ft_strndup(node->value, ft_strlen(node->value));
	else
	{
		if (chdir(current->value) != 0)
			return (write(2, "HOME not set\n", 14), g_err_state = 1, 1);
	}
	return (0);
}

static	int	cd_minus(t_token *current, t_data **data)
{
	t_env_list	*node;
	char		*tmp;
	char		*tmp2;

	node = (*data)->env_list;
	while (node && ft_strncmp(node->var, "OLDPWD=", 7) != 0)
		node = node->next;
	if (node)
	{
		tmp = ft_strndup(node->value, ft_strlen(node->value));
		tmp2 = current->value;
		current->value = ft_strjoin(tmp, current->value + 1);
		ft_free_null(tmp);
		ft_free_null(tmp2);
	}
	else
	{
		ft_free_null(current->value);
		return (1);
	}
	return (0);
}

static	int	cd_tilde(t_token *current, t_data **data)
{
	t_env_list	*node;
	char		*tmp;
	char		*tmp2;

	node = (*data)->env_list;
	while (node && ft_strncmp(node->var, "HOME=", 5) != 0)
		node = node->next;
	if (node)
	{
		tmp = ft_strndup(node->value, ft_strlen(node->value));
		tmp2 = current->value;
		current->value = ft_strjoin(tmp, current->value + 1);
		ft_free_null(tmp);
		ft_free_null(tmp2);
	}
	else
		return (write(2, "HOME not set\n", 14), g_err_state = 1, 1);
	return (0);
}

int	cd_cmd(t_data **data, t_token **tkn)
{
	t_token		*current;
	t_env_list	*node;

	current = (*tkn)->next;
	node = (*data)->env_list;
	if (check_err_tkn(tkn))
		return (g_err_state = 1, errno = 1,
			write(2, "too many arguments\n", 20), 1);
	node = (*data)->env_list;
	if (current->value[0] == '\0' && cd_null(current, data))
		return (1);
	if (current->value[0] == '-' && cd_minus(current, data))
		current->value = ft_strndup("", 1);
	if (current->value[0] == '~' && cd_tilde(current, data))
		return (1);
	if (chdir(current->value) != 0)
		return (ft_free_null(current->value), perror(""), g_err_state = 1, 0);
	chpwd(data);
	return (g_err_state = 0, 1);
}
