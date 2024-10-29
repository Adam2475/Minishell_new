/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 15:10:31 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cd_minus(t_token *current, t_data **data)
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

int	cd_tilde(t_token *current, t_data **data)
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
		return (write(2, "HOME not set\n", 14),
			(*data)->local_err_state = 1, 1);
	return (0);
}

int	cd_null(t_token *current, t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	while (node && ft_strncmp(node->var, "HOME=", 5) != 0)
		node = node->next;
	free(current->value);
	current->value = NULL;
	if (node)
		current->value = ft_strndup(node->value, ft_strlen(node->value));
	else if (!node)
		return (write(2, "HOME not set\n", 14),
			(*data)->local_err_state = 1, 1);
	else
		chdir(current->value);
	return (0);
}
