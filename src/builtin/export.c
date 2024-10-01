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

static	void	join_in_qt_exp(t_token *tkn, t_token_type type)
{
	t_token	*current;
	char	*tmp;
	int		i;

	i = 0;
	if (tkn->next->next && i == 0)
		current = tkn->next->next;
	else
		current = NULL;
	while (current->next && current->next->type != type)
	{
		tmp = current->value;
		current->value = ft_strjoin(current->value, current->next->value);
		free(tmp);
		tkn_delone(&current, current->next);
	}
	current = tkn;
	if (current->next && current->next->type == type && type != TOKEN_DOLLAR)
		tkn_delone(&current, current->next);
	while (current->next && current->next->type != type)
	{
		tmp = current->value;
		current->value = ft_strjoin(current->value, current->next->value);
		free(tmp);
		tkn_delone(&current, current->next);
	}
	if (current->next && current->next->type == type && type != TOKEN_DOLLAR)
		tkn_delone(&current, current->next);
	tmp = tkn->value;
	tkn->value = ft_strjoin(tkn->value, tkn->next->value);
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
			if (arg->next->type == TOKEN_DOUBLE_QUOTES || arg->next->type == TOKEN_DOLLAR
				|| arg->next->type == TOKEN_SINGLE_QUOTES)
				join_in_qt_exp(arg, arg->next->type);
			node->next = new_node_env(arg->value);
			free_node_env(node);
			return (0);
		}
		if (!node->next)
			break ;
		node = node->next;
	}
	if (arg->next->type == TOKEN_DOUBLE_QUOTES || arg->next->type == TOKEN_DOLLAR
		|| arg->next->type == TOKEN_SINGLE_QUOTES)
		join_in_qt_exp(arg, arg->next->type);
	node = new_node_env(arg->value);
	return (add_back_env(&(*data)->env_list, node), 0);
}


static	int	join_to_env(t_token *arg, t_data **data)
{
	t_env_list	*node;
	char		*tmp;
	char		*tmp2;

	node = (*data)->env_list;
	tmp = NULL;
	while (node)
	{
		if (ft_strncmp(node->var, arg->value, ft_strlen_char(arg->value, '+') - 1) == 0)
			break ;
		else if (!node->next)
			break ;
		node = node->next;
	}
	if (arg->next->type == TOKEN_DOUBLE_QUOTES || arg->next->type == TOKEN_DOLLAR
			|| arg->next->type == TOKEN_SINGLE_QUOTES)
		join_in_qt_exp(arg, arg->next->type);
	else if (!node->next && ft_strncmp(node->var, arg->value, ft_strlen_char(arg->value, '+') - 2) != 0)
	{
		tmp = ft_strndup(arg->value, ft_strlen_char(arg->value, '+') - 1);
		tmp2 = ft_strdup(ft_strnstr(arg->value, "=", ft_strlen_char(arg->value, '=')));
		free(arg->value);
		arg->value = ft_strjoin(tmp, tmp2);
		free(tmp);
		free(tmp2);
		add_to_env(arg, data);
		return (0);
	}
	tmp2 = ft_substr(arg->value, ft_strlen_char(arg->value, '='), ft_strlen(arg->value));
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

static int	util_exp(t_data **data, t_token **current, t_token **tkn)
{
	char	*var;
	int		flag;

	var = NULL;
	flag = 0;
	if ((*current)->value && !ft_isalpha((*current)->value[0]) && (*current)->type != TOKEN_WHITESPACE)
	{
		if (ft_strsearch((*current)->value, '='))
		{
			var = ft_strndup((*current)->value,
					(ft_strlen_char((*current)->value, '=') - 1));
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

static	int	inutil_exp(t_data **data, t_token **current, t_token **tkn, int *flag)
{
	while (current && (*current)->type != TOKEN_EOF)
	{
		while ((*current)->type != TOKEN_EOF && ((*current)->type == TOKEN_WHITESPACE
				|| ft_strncmp((*current)->value, "export", 6) == 0))
			(*current) = (*current)->next;
		if ((*current)->type != TOKEN_EOF && (*current)->type != TOKEN_DOLLAR
			&& util_exp(data, current, tkn) == 1)
			return (1);
		else if ((*current)->type != TOKEN_EOF && (*current)->type != TOKEN_DOLLAR
			&& util_exp(data, current, tkn) == 2)
			continue ;
		if ((*current)->value && ft_strsearch((*current)->value, '='))
		{
			if ((*current)->value[ft_strlen_char((*current)->value, '=') - 2] == '+')
				join_to_env((*current), data);
			else
				add_to_env((*current), data);
			(*flag) = 1;
		}
		(*current) = (*current)->next;
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
	flag = 0;
	if (inutil_exp(data, &current, &copy, &flag))
		return (g_err_state = 1, 	1);
	if (flag == 0)
		print_exp_env(data);
	free_list(copy);
	return (g_err_state = 0 ,1);
}
