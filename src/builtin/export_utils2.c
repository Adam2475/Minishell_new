/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/26 18:21:48 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_exp_env(t_data **data)
{
	t_env_list	*node;

	node = (*data)->env_list;
	while (node)
	{
		if (node->state == 0)
			ft_printf("declare -x %s\"%s\"\n", node->var, node->value);
		else if (node->state == 1)
			ft_printf("declare -x %s\n", node->var);
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
		if (node->type == 3 || node->type == 5
			|| node->type == 4 || node->type == 2
			|| node->type == 6)
			return (0);
		if (node->type != 12 && node->type != 11)
			return (1);
		node = node->next;
	}
	return (0);
}
