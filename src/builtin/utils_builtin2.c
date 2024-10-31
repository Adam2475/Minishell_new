/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtin2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 15:31:21 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_err_state(char *tmp, t_data **data)
{
	char	*err;
	char	*tmp2;

	if ((*data)->local_err_state > 256 || (*data)->local_err_state < 0)
		(*data)->local_err_state = (*data)->local_err_state % 255;
	if (g_err_state == 130)
		(*data)->local_err_state = 130;
	err = ft_itoa((*data)->local_err_state);
	tmp2 = ft_strtrim2(tmp, "=");
	tmp = ft_strtrim2(tmp2, "?");
	tmp2 = ft_strndup(tmp, ft_strlen(tmp));
	free(tmp);
	tmp = ft_strjoin(err, tmp2);
	free(tmp2);
	free(err);
	return (tmp);
}

static	int	ft_isalpha_len(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if ((str[i] <= 122 && str[i] >= 97) || (str[i] <= 90 && str[i] >= 65)
			|| (str[i] >= '0' && str[i] <= '9'))
			i++;
		else if (str[i] == '_')
			i++;
		else if (str[i] != '\0')
			return (i);
	}
	return (-1);
}

static	int	tmp_set2(char **tmp, char **tmp2, int *i, char *val)
{
	*tmp = ft_strndup(val, ft_strlen(val));
	*tmp2 = ft_strtrim2(*tmp, "$");
	if (**tmp2 && **tmp2 == '?')
		return (1);
	(*i) = ft_isalpha_len(*tmp2);
	if ((*i) > 0)
		*tmp = ft_strndup(*tmp2, (*i));
	else
		*tmp = ft_strndup(*tmp2, ft_strlen(*tmp2));
	return (0);
}

char	*tmp_set(char *val)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	if (tmp_set2(&tmp, &tmp2, &i, val))
		return (tmp2);
	if (i > 0 && tmp2[i] && tmp2[i] == '+'
		&& tmp2[i + 1] && tmp2[i + 1] == '=')
	{
		if (tmp2)
			free(tmp2);
		return (tmp2 = ft_strjoin(tmp, "="), ft_free_null(tmp), tmp2);
	}
	if (i > 0 && tmp2[i] && tmp2[i] != '=')
	{
		if (tmp)
			ft_free_null(tmp);
		tmp = ft_strtrim2(tmp2, &tmp2[i]);
		tmp2 = ft_strjoin(tmp, "=");
		return (ft_free_null(tmp), tmp2);
	}
	else if (i > 0 && tmp2[i] && tmp2[i] == '=')
		tmp = ft_strndup(tmp2, i + 1);
	return (ft_free_null(tmp2), tmp);
}
