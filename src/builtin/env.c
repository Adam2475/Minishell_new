/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:08:15 by marco             #+#    #+#             */
/*   Updated: 2024/09/15 17:55:29 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	env_cmd(t_data **data)
{
	t_env_list	*node;
	t_token		*tkn;

	node = (*data)->env_list;
	tkn = (*data)->tokens->next;
	while (tkn && tkn->type != 7 && tkn->type != 4
		&& tkn->type != 3 && tkn->type != 6)
	{
		if (tkn->type == TOKEN_WHITESPACE)
			tkn = tkn->next;
		else if (tkn->type != TOKEN_DOLLAR)
			return (g_err_state = 127,
				ft_printf("env: %s: No such file or directory\n",
					tkn->value));
		else if (tkn->type == TOKEN_DOLLAR)
			return (g_err_state = 126, ft_printf("env: %s: Permission denied\n",
					tkn->value));
	}
	while (node != NULL)
	{
		ft_printf("%s%s\n", node->var, node->value);
		node = node->next;
	}
	return (g_err_state = 0, 1);
}
