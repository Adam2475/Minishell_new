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

static	void	print_in_qt(t_token **node, t_token_type type)
{
	(*node) = (*node)->next;
	while ((*node)->type != type && (*node)->type != TOKEN_EOF)
	{
		ft_printf("%s", (*node)->value);
		(*node) = (*node)->next;
	}
	if ((int)(*node)->next->type == 11 && (*node)->type == type)
		ft_printf(" ");
}

static int	inutils_num(t_token *node)
{
	t_token	*tmp;
	int		flag;

	tmp = node;
	flag = 0;
	if (ft_strncmp(tmp->value, "-n", 3) == 0)
	{
		tmp = tmp->next;
		flag = 1;
	}
	return (flag);
}

int	echo_cmd(t_token **tkn)
{
	t_token	*node;
	int		flag_n;

	node = (*tkn)->next;
	flag_n = 0;
	while (node->type != TOKEN_EOF && node->type == TOKEN_WHITESPACE)
		node = node->next;
	flag_n = inutils_num(node);
	while (node && node->type != TOKEN_EOF)
	{
		if (((int)node->type == 8 || (int)node->type == 13)
			&& (int)node->next->type == 11)
			ft_printf("%s ", node->value);
		if (((int)node->type == 8 || (int)node->type == 13)
			&& (int)node->next->type != 11)
			ft_printf("%s", node->value);
		if ((int)node->type == 9 || (int)node->type == 10)
			print_in_qt(&node, node->type);
		node = node->next;
	}
	if (flag_n == 0)
		ft_printf("\n");
	return (g_err_state = 0, 1);
}

// TODO: problemi 'echo cioa disod ios'