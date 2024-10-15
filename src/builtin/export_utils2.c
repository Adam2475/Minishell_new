/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/07 12:28:02 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_exp_env(t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	while (node)
	{
		ft_printf("declare -x %s\"%s\"\n", node->var, node->value);
		if (node->next)
			node = node->next;
		else
			break ;
	}
}

int	check_for_flag(t_token **tkn)
{
	t_token	*node;

	node = *tkn;
	while (node && node->type != TOKEN_EOF)
	{
		if (node->type != 12 && node->type != 3
			&& node->type != 3 && node->type != 4
			&& node->type != 2 && node->type != 6)
			return (1);
		node = node->next;
	}
	return (0);
}
