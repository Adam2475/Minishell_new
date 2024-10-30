/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:44:57 by mapichec          #+#    #+#             */
/*   Updated: 2024/10/30 16:24:45 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_echo(t_token **node, int *flag_2)
{
	if ((*node)->type == 3 || (*node)->type == 5
		|| (*node)->type == 4)
	{
		(*flag_2) = 1;
		(*node) = (*node)->next;
	}
	if ((*node) && ((*node)->type == 13
			|| (*node)->type == 14) && (*flag_2) == 1)
	{
		(*flag_2) = 0;
		(*node) = (*node)->next;
	}
	if (((int)(*node)->type == 8 || (int)(*node)->type == 13
			|| (int)(*node)->type == 14 || (int)(*node)->type == 12)
		&& (int)(*node)->next->type == 11)
		ft_printf("%s ", (*node)->value);
	if (((int)(*node)->type == 8 || (int)(*node)->type == 13
			|| (int)(*node)->type == 14 || (int)(*node)->type == 12)
		&& (int)(*node)->next->type != 11)
		ft_printf("%s", (*node)->value);
}
