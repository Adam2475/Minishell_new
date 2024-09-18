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

	node = (*data)->env_list;
	while (node != NULL)
	{
		ft_printf("%s\n", node->content);
		node = node->next;
	}
	return (g_err_state = 0, 1);
}
