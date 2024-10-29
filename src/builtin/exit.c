/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 15:46:10 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	ft_is_numeric(t_token *tkn)
{
	int		i;
	char	*tmp;

	i = 0;
	if (!tkn->value || !tkn->value[0])
		return (0);
	if (tkn->value[0] == '-' || tkn->value[0] == '+')
	{
		i++;
		if (tkn->value[i] == '\0' && tkn->next->type == 14)
		{
			tmp = tkn->value;
			tkn->value = ft_strjoin(tkn->value, tkn->next->value);
			free(tmp);
			tkn_delone(&tkn, tkn->next);
		}
	}
	while (tkn->value[i])
	{
		if (!ft_isdigit(tkn->value[i]))
			return (0);
		i++;
	}
	return (1);
}

static	void	free_set_exit(t_data **data, t_token **token, t_token **tkn)
{
	while ((*tkn)->type == TOKEN_WHITESPACE || (*tkn)->type == 12)
		(*tkn) = (*tkn)->next;
	if ((int)(*tkn)->type == 7)
	{
		(*data)->local_err_state = 0;
		free_exit_cmd(data, *token);
	}
	if (!ft_is_numeric((*tkn)))
	{
		write(2, "exit: numeric argument required\n", 33);
		(*data)->local_err_state = 2;
		free_exit_cmd(data, *token);
	}
	if (ft_too_long((*tkn)->value, data))
	{
		(*data)->local_err_state = 1;
		free_exit_cmd(data, *token);
	}
}

int	cmd_exit(t_data **data, t_token **token)
{
	t_token		*tkn;

	tkn = (*token)->next;
	free_set_exit(data, token, &tkn);
	if (ft_is_numeric(tkn))
	{
		while ((int)tkn->type != 7 || (int)tkn->type == 11)
		{
			tkn = tkn->next;
			if (tkn->type != 11 && tkn->type != 7)
				break ;
		}
		if ((int)tkn->type != 7)
			return ((*data)->local_err_state = 1,
				write(2, "exit: too many arguments\n", 26), 1);
		free_exit_cmd(data, *token);
	}
	return ((*data)->local_err_state = 0, 0);
}
