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

int	pwd_cmd(t_data **data)
{
	t_env_list	*node;
	t_token		*tkn;

	node = (*data)->env_list;
	tkn = (*data)->tokens->next;
	while (tkn && tkn->type != TOKEN_EOF)
	{
		if (tkn->type == TOKEN_WHITESPACE)
			tkn = tkn->next;
		else
			return (ft_printf("pwd: too many arguments\n"));
	}
	while (node && ft_strncmp(node->var, "PWD=", 4))
		node = node->next;
	if (node)
		ft_printf("%s\n", node->value);
	return (g_err_state = 0, 1);
}
