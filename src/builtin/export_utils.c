/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/26 18:28:02 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	ft_isalpha_plus(char *str)
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

static	void	add_for_exp(t_data **data, char *arg)
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
		return (unset_env(tkn, &(*data)->env_list), 1);
	if ((*current)->value && (*current)->type != 11 && ft_strsearch((*current)->value, '=') == 0)
		return (add_for_exp(data, (*current)->value), (*current) = (*current)->next, 2);
	return (0);
}

static	int	inutil_exp2(t_token **current, t_data **data)
{
	if ((*current)->value && ft_strsearch((*current)->value, '='))
	{
		if ((*current)->value[ft_strlen_char((*current)->value,
					'=') - 2] == '-')
			return (1);
		if ((*current)->value[ft_strlen_char((*current)->value,
					'=') - 2] == '+')
			join_to_env((*current), data);
		else
			add_to_env((*current), data);
	}
	return (0);
}

int	inutil_exp(t_data **data, t_token **current, t_token **tkn)
{
	while (current && (*current) && (*current)->type != TOKEN_EOF)
	{
		while ((*current)->type != TOKEN_EOF
			&& ((*current)->type == TOKEN_WHITESPACE
				|| ft_strncmp((*current)->value, "export", 6) == 0))
			(*current) = (*current)->next;
		if (((*current)->type > 7 || (*current)->type < 2)
			&& util_exp(data, current, tkn) == 1)
			return (1);
		else if (((*current)->type > 7 || (*current)->type < 2)
			&& util_exp(data, current, tkn) == 2)
			continue ;
		if (inutil_exp2(current, data))
			return (1);
		(*current) = (*current)->next;
	}
	return (0);
}

void	join_in_qt_exp(t_token *tkn)
{
	t_token	*current;
	char	*tmp;

	if (tkn->next && tkn->next->type == 14)
		current = tkn->next;
	else
		current = NULL;
	while (current && current->next && current->next->type != 11
		&& current->next->type != 7)
	{
		tmp = current->value;
		current->value = ft_strjoin(current->value, current->next->value);
		free(tmp);
		tkn_delone(&current, current->next);
	}
	tmp = tkn->value;
	tkn->value = ft_strjoin(tkn->value, tkn->next->value);
	free(tmp);
	tkn_delone(&tkn, tkn->next);
	return ;
}
