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


int	is_numeric(char *str)
{
	int i = 0;

	if (!str || !str[0])
		return 0;
	if (str[0] == '-')
		i++;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	unset_env(t_env_list **env, char *var_name)
{
	t_env_list *current = *env;

	var_name = ft_strjoin(var_name, "=");
	while (current && ft_strncmp(current->var, var_name, ft_strlen(var_name)) != 0)
	{
		if (current->next)
			current = current->next;
		else
			break ;
	}
	if (ft_strncmp(current->var, var_name, ft_strlen(var_name)) == 0)
	{
		if (current->pre)
			current->pre->next = current->next;
		else
			*env = current->next;
		if (current->next)
			current->next->pre = current->pre;
		free(current->var);
		free(current->value);
		free(current->content);
		free(current);
		return (1);
	}
	return (0);
}
