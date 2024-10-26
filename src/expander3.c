/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/15 15:22:09 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*ft_set_zero(t_token *current, int flag)
{
	while ((int)current->type != 7 && flag == 0
		&& (int)current->type != 10)
	{
		current->type = TOKEN_WORD_QT;
		current = current->next;
	}
	while ((int)current->type != 7 && (int)current->type != 9
		&& flag == 1)
	{
		if ((int)current->type != 8)
			current->type = TOKEN_WORD_QT;
		else if ((int)current->type == 8
			&& (int)current->next->type == 13)
			current = current->next;
		current = current->next;
	}
	return (current);
}
