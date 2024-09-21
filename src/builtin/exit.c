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

static	void	join_in_qt(t_data **data, t_token *tkn, t_token_type type, int flag)
{
	t_token	*current;
	char	*tmp;

	current = tkn;
	while (current->next && current->next->type != type && flag == 0)
	{
		tmp = current->value;
		current->value = ft_strjoin(current->value, current->next->value);
		free(tmp);
		tkn_delone(&current, current->next);
	}
	ft_printf("exit: %s: numeric argument required\n", tkn->value);
	free_exit(data, (*data)->tokens);
	return ;
}

int	cmd_exit(t_data **data)
{
	t_token	*tkn;
	t_token	*tmp;

	tkn = (*data)->tokens->next;
	tmp = NULL;
	ft_printf("exit\n");
	while (tkn->type == TOKEN_WHITESPACE)
		tkn = tkn->next;
	if ((int)tkn->type == 7)
		free_exit(data, (*data)->tokens);
	if (tkn->type == TOKEN_DOUBLE_QUOTES || TOKEN_SINGLE_QUOTES)
		join_in_qt(data, tkn->next, tkn->type, 0);
	if (!is_numeric(tkn->value))
		join_in_qt(data, tkn->next, tkn->type, 1);
	if (is_numeric(tkn->value))
	{
		tmp = tkn;
		while ((int)tkn->type != 7 && (int)tkn->type == 11)
			tkn = tkn->next;
		if ((int)tkn->type != 7)
			return (ft_printf("bash: exit: too many arguments\n"));
		free_exit(data, (*data)->tokens);
	}
	return (0);
}
