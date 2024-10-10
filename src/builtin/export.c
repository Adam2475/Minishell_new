/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c	                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	add_to_env(t_token *arg, t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	while (node)
	{
		if (ft_strncmp(arg->value, node->var,
				(ft_strlen_char(arg->value, '=') - 1)) == 0)
		{
			if (arg->next->type == 14 || arg->next->type == 8)
				join_in_qt_exp(arg);
			node->next = new_node_env(arg->value);
			free_node_env(node);
			return (0);
		}
		if (!node->next)
			break ;
		node = node->next;
	}
	if (arg->next->type == 14 || arg->next->type == 8)
		join_in_qt_exp(arg);
	node = new_node_env(arg->value);
	return (add_back_env(&(*data)->env_list, node), 0);
}

static	int	util_join_to_env(t_env_list *node, t_token *arg, t_data **data)
{
	char		*tmp;
	char		*tmp2;

	if (arg->next->type == 14 || arg->next->type == 8)
		join_in_qt_exp(arg);
	else if (!node->next && ft_strncmp(node->var, arg->value,
			ft_strlen_char(arg->value, '+') - 2) != 0)
	{
		tmp = ft_strndup(arg->value,
				ft_strlen_char(arg->value, '+') - 1);
		tmp2 = ft_strdup(ft_strnstr(arg->value, "=",
					ft_strlen_char(arg->value, '=')));
		free(arg->value);
		arg->value = ft_strjoin(tmp, tmp2);
		free(tmp);
		free(tmp2);
		add_to_env(arg, data);
		return (0);
	}
	return (1);
}

int	join_to_env(t_token *arg, t_data **data)
{
	t_env_list	*node;
	char		*tmp;
	char		*tmp2;

	node = (*data)->env_list;
	tmp = NULL;
	while (node)
	{
		if (ft_strncmp(node->var, arg->value,
				ft_strlen_char(arg->value, '+') - 1) == 0)
			break ;
		else if (!node->next)
			break ;
		node = node->next;
	}
	if (!util_join_to_env(node, arg, data))
		return (0);
	tmp2 = ft_substr(arg->value, ft_strlen_char(arg->value, '='),
			ft_strlen(arg->value));
	tmp = node->value;
	node->value = ft_strjoin(node->value, tmp2);
	free(tmp);
	free(tmp2);
	return (0);
}

static	void	print_exp_env(t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	while (node)
	{
		ft_printf("declare -x %s\"%s\"\n", node->var, node->value);
		if (node->next)
			node = node->next;
		else
			break ;
	}
}

static	int	check_for_flag(t_token **tkn)
{
	t_token	*node;

	node = *tkn;
	while (node && node->type != TOKEN_EOF)
	{
		if (node->type != 12 && node->type != 3
			&& node->type != 3 && node->type != 4
			&& node->type != 2 && node->type != 6)
			return (1);
		node = node->next;
	}
	return (0);
}

int	export_cmd(t_data **data, t_token **tkn)
{
	t_token		*current;
	t_token		*copy;
	int			flag;

	copy = copy_token_list(data, (*tkn));
	current = copy;
	flag = check_for_flag(tkn);
	if (inutil_exp(data, &current, &copy))
		return (write(2, "not a valid identifier\n", 24),
				g_err_state = 1, errno = 1, 1);
	if (flag == 0)
		print_exp_env(data);
	free_list(copy);
	return (g_err_state = 0, 1);
}
