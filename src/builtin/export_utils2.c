/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 15:15:01 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_exp_env(t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	while (node)
	{
		if (node->state == 0)
			ft_printf("declare -x %s\"%s\"\n", node->var, node->value);
		else if (node->state == 1)
			ft_printf("declare -x %s\n", node->var);
		if (node->next)
			node = node->next;
		else
			break ;
	}
}

int	check_for_flag(t_token **tkn)
{
	t_token	*node;

	node = *tkn;
	while (node && node->type != TOKEN_EOF)
	{
		if (node->type == 3 || node->type == 5
			|| node->type == 4 || node->type == 2
			|| node->type == 6)
			return (0);
		if (node->type != 12 && node->type != 11)
			return (1);
		node = node->next;
	}
	return (0);
}

int	ft_isalpha_plus(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] <= 122 && str[i] >= 97) || (str[i] <= 90 && str[i] >= 65))
			i++;
		else if (str[i] == '_')
			i++;
		else if (str[i] != '\0' && str[i] != '=' && str[i] != '+')
			return (1);
		else if (str[0] == '=' || str[0] == '+')
			return (1);
		else if (str[i] == '+' && str[i + 1] == '=')
			return (0);
		else if (str[i] == '+' && str[i + 1] != '=')
			return (1);
		else if (str[i] == '\0' || str[i] == '=')
			return (0);
	}
	return (0);
}

void	add_for_exp(t_data **data, char *arg)
{
	t_env_list	*node;
	t_env_list	*new;

	node = (*data)->env_list;
	while (node)
	{
		if (!ft_strncmp(node->var, arg, ft_strlen(node->var)))
			return ;
		if (!node->next)
			break ;
		node = node->next;
	}
	if (!node->next && ft_strncmp(node->var, arg, ft_strlen(node->var)))
	{
		new = (t_env_list *)ft_calloc(sizeof(t_env_list), 1);
		new->var = ft_strndup(arg, ft_strlen(arg));
		new->value = ft_calloc(1, 1);
		new->content = ft_calloc(1, 1);
		new->state = 1;
		node->next = new;
		new->pre = node;
	}
}

int	util_exp(t_data **data, t_token **current, t_token **tkn)
{
	if ((*current)->value && ft_isalpha_plus((*current)->value)
		&& (*current)->type != TOKEN_WHITESPACE)
		return (unset_env(tkn, &(*data)->env_list, data), 1);
	if ((*current)->value && (*current)->type != 11
		&& ft_strsearch((*current)->value, '=') == 0)
		return (add_for_exp(data, (*current)->value),
			(*current) = (*current)->next, 2);
	return (0);
}
