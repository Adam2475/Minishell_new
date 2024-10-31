/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 14:59:04 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	inutil_exp2(t_token **current, t_data **data)
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
