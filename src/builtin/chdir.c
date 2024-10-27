/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/27 16:27:50 by mapichec         ###   ########.fr       */
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

int	cd_cmd(t_data **data, t_token **tkn)
{
	t_token		*current;

	current = (*tkn)->next;
	if (check_err_tkn(tkn))
		return ((*data)->local_err_state = 1,
			write(2, "too many arguments\n", 20), 1);
	if (current->value[0] == '\0' && cd_null(current, data))
		return (1);
	if (current->value[0] == '-' && cd_minus(current, data))
		current->value = ft_strndup("", 1);
	if (current->value[0] == '~' && cd_tilde(current, data))
		return (1);
	if (chdir(current->value) != 0)
		return (perror(""), (*data)->local_err_state = 1, 0);
	chpwd(data);
	return ((*data)->local_err_state = 0, 1);
}
