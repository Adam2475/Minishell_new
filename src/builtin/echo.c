/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 16:32:37 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	heredoc_unlink(t_data **data)
{
	g_err_state = 0;
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

	if (!tmp || (tmp->value && tmp->type == 14 && !tmp->value[0]))
		return ;
	n_s = tmp->value;
	if (tmp->type == 9 || tmp->type == 10 || tmp->type == 8)
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

static	void	skip_prob(t_token **node)
{
	if ((*node)->type != 7 && strncmp((*node)->value, "echo", 4) == 0)
		(*node) = (*node)->next;
	while ((*node) && (*node)->type != 7
		&& ((*node)->type == 11 || (*node)->type == 12))
		(*node) = (*node)->next;
	if ((*node)->type <= 6 && (*node)->type >= 3)
	{
		while ((*node) && (*node)->type != 13)
			(*node) = (*node)->next;
		if ((*node) && (*node)->type != 7)
			(*node) = (*node)->next;
		while ((*node) && (*node)->type != 7 && (*node)->type == 11)
			(*node) = (*node)->next;
	}
}

int	echo_cmd(t_token **tkn, t_data **data)
{
	t_token	*node;
	int		flag_n;
	int		flag_2;

	node = (*tkn)->next;
	flag_n = 0;
	flag_2 = 0;
	skip_prob(&node);
	inutils_num(node, &flag_n);
	while (node && node->type != 7 && node->type != 6 && node->type != 2)
	{
		print_echo(&node, &flag_2);
		node = node->next;
	}
	if (flag_n == 0)
		write(1, "\n", 2);
	return ((*data)->local_err_state = 0, 1);
}
