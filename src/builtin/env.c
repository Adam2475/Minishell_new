/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:08:15 by marco             #+#    #+#             */
/*   Updated: 2024/10/30 19:42:52 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	env_help(t_env_list *node)
{
	if (node->state == 0 && node->value && node->value)
	{
		write(1, node->var, ft_strlen(node->var));
		write(1, node->value, ft_strlen(node->value));
		write(1, "\n", 1);
	}
}

int	env_cmd(t_data **data, t_token **token)
{
	t_env_list	*node;
	t_token		*tkn;

	node = (*data)->env_list;
	tkn = (*token)->next;
	(*data)->merdoso = 1;
	while (tkn && tkn->type != 7 && tkn->type != 4 && tkn->type != 5
		&& tkn->type != 3 && tkn->type != 6 && tkn->type != 2)
	{
		if (tkn->type == TOKEN_WHITESPACE || tkn->type == 12)
			tkn = tkn->next;
		else if (tkn->type != TOKEN_DOLLAR && tkn->type != 2)
			return ((*data)->local_err_state = 127,
				write(2, "Not file or directory\n", 23), 0);
		else if (tkn->type == TOKEN_DOLLAR)
			return ((*data)->local_err_state = 126,
				write(2, "Permission denied\n", 19), 0);
	}
	while (node != NULL)
	{
		env_help(node);
		node = node->next;
	}
	return ((*data)->local_err_state = 0, 1);
}
