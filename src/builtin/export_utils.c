/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	util_exp(t_data **data, t_token **current, t_token **tkn)
{
	char	*var;
	int		flag;

	var = NULL;
	flag = 0;
	if ((*current)->value && !ft_isalpha((*current)->value[0])
		&& (*current)->value[0] != '_' && (*current)->type != TOKEN_WHITESPACE)
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

// static	int	ft_join_val(t_token *current, t_token *join)
// {
// 	char	*tmp;

// 	tmp = current->value;
// 	if (current && join && join->type != 7)
// 	{
// 		current->value = ft_strjoin(current->value, join->value);
// 		ft_free_null(tmp);
// 		tkn_delone(&current, join);
// 		return (0);
// 	}
// 	return (1);
// }

int	inutil_exp(t_data **data, t_token **current, t_token **tkn, int *flag)
{
	while (current && (*current) && (*current)->type != TOKEN_EOF)
	{
		while ((*current)->type != TOKEN_EOF
			&& ((*current)->type == TOKEN_WHITESPACE
				|| ft_strncmp((*current)->value, "export", 6) == 0))
			(*current) = (*current)->next;
		if ((*current)->type != 7 && (*current)->type != 8
			&& util_exp(data, current, tkn) == 1)
			return (1);
		else if ((*current)->type != 7 && (*current)->type != 8
			&& util_exp(data, current, tkn) == 2)
			continue ;
		if ((*current)->value && ft_strsearch((*current)->value, '='))
		{
			if ((*current)->value[ft_strlen_char((*current)->value,
						'=') - 2] == '+')
				join_to_env((*current), data);
			else
				add_to_env((*current), data);
			(*flag) = 1;
		}
		(*current) = (*current)->next;
	}
	return (0);
}

// void	util_join_in_qt(t_token *tkn, t_token *current,
// 	t_token_type type, char *tmp)
// {
// 	current = tkn;
// 	if (current->next && current->next->type == type && type != TOKEN_DOLLAR)
// 		tkn_delone(&current, current->next);
// 	while (current->next && current->next->type != type)
// 	{
// 		tmp = current->value;
// 		current->value = ft_strjoin(current->value, current->next->value);
// 		free(tmp);
// 		tkn_delone(&current, current->next);
// 	}
// 	if (current->next && current->next->type == type && type != TOKEN_DOLLAR)
// 		tkn_delone(&current, current->next);
// }

void	join_in_qt_exp(t_token *tkn)
{
	t_token	*current;
	char	*tmp;

	if (tkn->next && tkn->next->type == 14)
		current = tkn->next;
	else
		current = NULL;
	while (current->next && current->next->type != 11 && current->next->type != 7)
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
