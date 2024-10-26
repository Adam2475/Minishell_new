/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 20:34:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/08/28 17:56:44 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../inc/minishell.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*nodo;

	nodo = malloc(sizeof(t_list));
	if (!nodo)
		return (0);
	nodo->content = content;
	nodo->next = 0;
	return (nodo);
}

t_token	*ft_lstnewtoken(t_token_type type, char *content)
{
	t_token	*nodo;

	nodo = (t_token *)malloc(sizeof(t_token));
	if (!nodo)
		return (0);
	nodo->state = STATE_NORMAL;
	if (type == TOKEN_DOLLAR && content[1] == '\0')
		nodo->type = 0;
	else
		nodo->type = type;
	nodo->value = content;
	nodo->next = 0;
	return (nodo);
}
