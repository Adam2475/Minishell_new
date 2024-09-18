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

	node = (*data)->env_list;
	while (ft_strncmp(node->var, "PWD=", 4))
		node = node->next;
	ft_printf("%s\n", node->value);
	return (g_err_state = 0, 1);
}
