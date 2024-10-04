/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/03 18:47:19 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_err_state(char *tmp)
{
	char	*err;
	char	*tmp2;

	err = ft_itoa(g_err_state);
	tmp2 = ft_strtrim2(tmp, "=");
	tmp = ft_strtrim2(tmp2, "?");
	tmp2 = ft_strndup(tmp, ft_strlen(tmp));
	free(tmp);
	tmp = ft_strjoin(err, tmp2);
	free(tmp2);
	free(err);
	return (tmp);
}

char	*tmp_set(char *val)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strndup(val, ft_strlen(val));
	tmp2 = ft_strtrim2(tmp, "$");
	tmp = ft_strjoin(tmp2, "=");
	ft_free_null(tmp2);
	return (tmp);
}

void	process_command2(t_data **data, char **command)
{
	if (!(*data)->merdoso)
		(*data)->cmd2 = find_cmd(command[0], data);
	else
		(*data)->cmd2 = ft_strndup((*data)->input, ft_strlen((*data)->input));
}
