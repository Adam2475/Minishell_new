/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/03 18:35:32 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	free_exit_cmd(t_data **data, t_token *tokens)
{
	if ((*data)->tmp9)
		free((*data)->tmp9);
	free_tokens(data, tokens);
	if ((*data)->env_list)
		free_env_list((*data)->env_list);
	free(*data);
	exit(1);
}

void	join_in_qt(t_token *tkn,
			t_token_type type, int flag)
{
	t_token	*current;
	char	*tmp;

	current = tkn;
	while (current->next && current->next->type != type && flag == 0)
	{
		tmp = current->value;
		current->value = ft_strjoin(current->value, current->next->value);
		free(tmp);
		tkn_delone(&current, current->next);
	}
	ft_printf("exit: %s: numeric argument required\n", tkn->value);
	return ;
}

static void	*ft_check_lon(char *s)
{
	static long		result = 0;
	static int		digit = 0;

	if (*s == '-')
		s++;
	else if (*s == '+')
		s++;
	while ((*s) && ft_isdigit(*s))
	{
		digit = *s - '0';
		if (result > (LONG_MAX - digit) / 10)
			return ((void *)s);
		result = result * 10 + digit;
		s++;
	}
	return (NULL);
}

static	int	ft_too_long(char *val, t_data **data, t_token **token)
{
	char	*ptr;

	ptr = ft_strndup(val, ft_strlen(val));
	if (!val)
		return (0);
	if ((ft_strlen(val) - 1) > ft_strlen("-9223372036854775808"))
		return (ft_printf("exit: %s: numeric argument required 1\n", val),
			free_exit_cmd(data, *token), 1);
	if (ft_check_lon(ptr) != NULL)
		return (ft_printf("exit: %s: numeric argument required 2\n", val),
			free_exit_cmd(data, *token), 1);
	if (ft_atol(val) > 255)
		g_err_state = ft_atol(val) / 256;
	else
		g_err_state = ft_atol(val);
	return (0);
}

int	cmd_exit(t_data **data, t_token **token)
{
	t_token		*tkn;

	tkn = (*token)->next;
	while (tkn->type == TOKEN_WHITESPACE)
		tkn = tkn->next;
	if ((int)tkn->type == 7)
		free_exit_cmd(data, *token);
	if (tkn->type == TOKEN_DOUBLE_QUOTES || tkn->type == TOKEN_SINGLE_QUOTES)
		(join_in_qt(tkn->next, tkn->type, 0),
			free_exit_cmd(data, *token));
	if (!is_numeric(tkn->value) || ft_too_long(tkn->value, data, token))
		join_in_qt(tkn, tkn->type, 1);
	if (is_numeric(tkn->value))
	{
		while ((int)tkn->type != 7 || (int)tkn->type == 11)
		{
			tkn = tkn->next;
			if (tkn->type != 11 && tkn->type != 7)
				break ;
		}
		if ((int)tkn->type != 7)
			return (ft_printf("exit\nbash: exit: too many arguments\n"));
		free_exit_cmd(data, *token);
	}
	return (0);
}
