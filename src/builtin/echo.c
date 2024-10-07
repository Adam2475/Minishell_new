/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/07 16:07:15 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	heredoc_unlink(t_data **data)
{
	if ((*data)->heredoc_flag > 0)
	{
		(*data)->heredoc_flag = 0;
		unlink(".heredoc.txt");
	}
}

static	void	do_some(t_token **tmp, char **n_s)
{
	(*tmp)->type = TOKEN_OPTION;
	(*tmp) = (*tmp)->next;
	while ((*tmp)->type != 7 && (*tmp)->type == 11)
		(*tmp) = (*tmp)->next;
	(*n_s) = (*tmp)->value;
}

static void	inutils_num(t_token *tmp, int *flag_n)
{
	char	*n_s;

	n_s = tmp->value;
	if (tmp->type == 9 || tmp->type == 10)
		return ;
	while (tmp && tmp->type != 7)
	{
		if (ft_strncmp(n_s, "-n", 1) == 0)
			n_s++;
		if (*n_s && *n_s == 'n')
		{
			n_s++;
			continue ;
		}
		else if (!*n_s || *n_s == '\0')
		{
			do_some(&tmp, &n_s);
			(*flag_n) += 1;
		}
		else if (*n_s && *n_s != '\0' && *n_s != 'n')
			break ;
	}
}

int	echo_cmd(t_token **tkn)
{
	t_token	*node;
	int		flag_n;

	node = (*tkn)->next;
	flag_n = 0;
	while (node->type != 7 && (node->type == 11 || node->type == 12))
		node = node->next;
	inutils_num(node, &flag_n);
	node = (*tkn)->next;
	while (node && node->type != 7 && node->type != 3
			&& node->type != 4 && node->type != 6)
	{
		if (((int)node->type == 8 || (int)node->type == 13
				|| (int)node->type == 14)
			&& (int)node->next->type == 11)
			ft_printf("%s ", node->value);
		if (((int)node->type == 8 || (int)node->type == 13
				|| (int)node->type == 14)
			&& (int)node->next->type != 11)
			ft_printf("%s", node->value);
		node = node->next;
	}
	if (flag_n == 0)
		ft_printf("\n");
	return (g_err_state = 0, 1);
}
