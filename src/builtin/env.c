/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:08:15 by marco             #+#    #+#             */
/*   Updated: 2024/10/10 17:59:02 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	env_cmd(t_data **data)
{
	t_env_list	*node;
	t_token		*tkn;

	node = (*data)->env_list;
	tkn = (*data)->tokens->next;
	(*data)->merdoso = 1;
	while (tkn && tkn->type != 7 && tkn->type != 4 && tkn->type != 5
		&& tkn->type != 3 && tkn->type != 6 && tkn->type != 2)
	{
		if (tkn->type == TOKEN_WHITESPACE)
			tkn = tkn->next;
		else if (tkn->type != TOKEN_DOLLAR)
			return (g_err_state = 127, errno = 127,
					write(2, "Not file or directory\n", 23), 0);
		else if (tkn->type == TOKEN_DOLLAR)
			return (g_err_state = 126, errno = 126,
					write(2, "Permission denied\n", 19), 0);
	}
	while (node != NULL)
	{
		ft_printf("%s%s\n", node->var, node->value);
		node = node->next;
	}
	return (g_err_state = 0, errno = 0, 1);
}
