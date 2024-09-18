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

int	echo_cmd(t_token **tkn)
{
	t_token	*node;

	node = (*tkn);
	while (node)
	{
		if ((int)node->type == 0 && (int)node->next->type == 11)
		{
			ft_printf("%s ", node->value);
			if (node->next->next)
			{
				node = node->next->next;
				continue ;
			}
		}
		else if ((int)node->type == 0 || (int)node->type == 14)
		{
			ft_printf("%s", node->value);
			node = node->next;
		}
		if ((int)node->type == 7)
			return (g_err_state = 0, ft_printf("\n"));
	}
	g_err_state = 0;
	return (1);
}
