/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexers_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	join_in_qt_exp(t_data **data, t_token *tkn, t_token_type type)
{
	t_token	*current;
	char	*tmp;

	current = tkn->next->next;
	while (current->next && current->next->type != type)
	{
		tmp = current->value;
		current->value = ft_strjoin(current->value, current->next->value);
		free(tmp);
		tkn_delone(&current, current->next);
	}
	current = tkn;
	tkn_delone(&current, current->next);
	while (current->next->type != type)
		current = current->next;
	tkn_delone(&current, current->next);
	tmp = tkn->value;
	tkn->value = ft_strjoin(tkn->value, tkn->next->value)	;
	free(tmp);
	return ;
}

int	add_to_env(t_token *arg, t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	while (node)
	{
		if (ft_strncmp(arg->value, node->var,
			(ft_strlen_char(arg->value, '=') - 1)) == 0)
		{
			if (arg->next->type == TOKEN_DOUBLE_QUOTES
				|| arg->next->type ==  TOKEN_SINGLE_QUOTES)
				join_in_qt_exp(data, arg, arg->next->type);
			node->next = new_node_env(arg->value);
			free_node_env(node);
			return (0);
		}
		if (!node->next)
			break ;
		node = node->next;
	}
	if (arg->next->type == TOKEN_DOUBLE_QUOTES
		|| arg->next->type ==  TOKEN_SINGLE_QUOTES)
		join_in_qt_exp(data, arg, arg->next->type);
	node = new_node_env(arg->value);
	return (add_back_env(&(*data)->env_list, node), 0);
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

static int	util_exp(t_data **data, t_token **current, t_token **tkn)
{
	char	*var;
	int		flag;

	var = NULL;
	flag = 0;
	if ((*current)->value && !(((*current)->value[0] >= 'a' && (*current)->value[0] <= 'z')
			|| ((*current)->value[0] >= 'A' && (*current)->value[0] <= 'Z') || (*current)->value[0] == 32))
	{
		if (ft_strsearch((*current)->value, '='))
		{
			var = ft_strndup((*current)->value, (ft_strlen_char((*current)->value, '=') - 1));
			flag = 1;
		}
		else
			var = (*current)->value;
		ft_printf("bash: export: `%s': not a valid identifier\n", var);
		if (flag == 1)
			free(var);
		return (unset_env(tkn, &(*data)->env_list), 1);			
	}
	if ((*current)->value && ft_strsearch((*current)->value, '=') == 0)
		return ((*current) = (*current)->next, 2);
	return (0);
}

int	export_cmd(t_data **data, t_token **tkn)
{
	t_token		*current;
	int			flag;

	current = (*tkn);
	flag = 0;
	while (current->value && current->type != TOKEN_EOF)
	{
		while (current->type != TOKEN_EOF && current->type == TOKEN_WHITESPACE)
			current = current->next;
		if (util_exp(data, &current, tkn) == 1)
			return (1);
		else if (util_exp(data, &current, tkn) == 2)
			continue ;
		if (current->value && ft_strsearch(current->value, '='))
		{
			add_to_env(current, data);
			flag = 1;
		}
		current = current->next;
	}
	if (flag == 0)
		print_exp_env(data);
	return(1);
}
