/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 15:23:31 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	free_exit_cmd2(t_data **data)
{
	if ((*data)->tmp9 != NULL)
		free((*data)->tmp9);
	if ((*data)->tmp6)
		free((*data)->tmp6);
	if ((*data)->env_p && (*data)->pipes)
		free_char_array((*data)->env_p);
	if ((*data)->cmd_args_exit)
		free_char_array((*data)->cmd_args_exit);
}

void	free_exit_cmd(t_data **data, t_token *tokens)
{
	int	tmp;

	tmp = 0;
	free_exit_cmd2(data);
	if ((*data)->tokens_ptr != NULL)
	{
		free_list((*data)->tokens_ptr);
		(*data)->tokens_ptr = NULL;
	}
	if ((*data)->end)
		free((*data)->end);
	if ((*data)->command2)
		free((*data)->command2);
	if (!(*data)->pipes)
		free_tokens(data, tokens);
	if ((*data)->pipes)
	{
		free_tokens(data, (*data)->tokens_ptr);
		tokens = NULL;
	}
	if ((*data)->env_list)
		free_env_list((*data)->env_list);
	tmp = (*data)->local_err_state;
	free(*data);
	exit(tmp);
}

void	*ft_check_lon(char *s)
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

int	ft_too_long(char *val, t_data **data)
{
	char	*ptr;

	ptr = ft_strndup(val, ft_strlen(val));
	if (!val)
		return (0);
	if ((ft_strlen(val) - 1) > ft_strlen("-9223372036854775808"))
		return (free(ptr),
			write(2, "exit: numeric argument required\n", 33), 1);
	if (ft_check_lon(ptr) != NULL)
		return (free(ptr),
			write(2, "exit: numeric argument required\n", 33), 1);
	if (ft_atol(val) > 255 || ft_atol(val) < 0)
		(*data)->local_err_state = ft_atol(val) % 256;
	else
		(*data)->local_err_state = ft_atol(val);
	free(ptr);
	return (0);
}
