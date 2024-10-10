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
	exit(g_err_state);
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
		return (write(2, "exit: numeric argument required\n", 33),
			free_exit_cmd(data, *token), 1);
	if (ft_check_lon(ptr) != NULL)
		return (write(2, "exit: numeric argument required\n", 33),
			free_exit_cmd(data, *token), 1);
	if (ft_atol(val) > 255 || ft_atol < 0)
		g_err_state = ft_atol(val) % 256;
	else
		g_err_state = ft_atol(val);
	errno = g_err_state;
	return (0);
}

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

int	cmd_exit(t_data **data, t_token **token)
{
	t_token		*tkn;

	tkn = (*token)->next;
	while (tkn->type == TOKEN_WHITESPACE)
		tkn = tkn->next;
	if ((int)tkn->type == 7)
		free_exit_cmd(data, *token);
	if (!ft_is_numeric(tkn))
	{
		write(2, "exit: numeric argument required\n", 33);
		g_err_state = 2;
		errno = 2;
		free_exit_cmd(data, *token);
	}
	if (ft_too_long(tkn->value, data, token))
	{
		g_err_state = 100;
		errno = 100;
		free_exit_cmd(data, *token);
	}
	if (ft_is_numeric(tkn))
	{
		while ((int)tkn->type != 7 || (int)tkn->type == 11)
		{
			tkn = tkn->next;
			if (tkn->type != 11 && tkn->type != 7)
				break ;
		}
		if ((int)tkn->type != 7)
			return (g_err_state = 1, write(2, "exit: too many arguments\n", 26));
		free_exit_cmd(data, *token);
	}
	return (errno = 0, 0);
}
