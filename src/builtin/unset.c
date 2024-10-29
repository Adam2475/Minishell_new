/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c	                                        :+:      :+:    :+:   */
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
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
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

void	roll_env(t_env_list **current, char *var_name)
{
	while ((*current))
	{
		if (ft_strncmp((*current)->var, var_name,
				ft_strlen((*current)->var) - 1) == 0)
		{
			free_node_env((*current));
			break ;
		}
		else if ((*current)->next)
			(*current) = (*current)->next;
		else
			break ;
	}
}

int	unset_env(t_token **token, t_env_list **env, t_data **data)
{
	t_env_list	*current;
	t_token		*tkn;
	char		*var_name;

	tkn = (*token)->next;
	var_name = NULL;
	while (tkn && tkn->type != TOKEN_EOF)
	{
		current = *env;
		while (tkn && tkn->next && tkn->type == TOKEN_WHITESPACE)
			tkn = tkn->next;
		if (tkn->type == 7)
			break ;
		if (ft_strsearch(tkn->value, '='))
			var_name = ft_strndup(tkn->value, ft_strlen_char(tkn->value, '='));
		else
			var_name = ft_strjoin(tkn->value, "=");
		roll_env(&current, var_name);
		free(var_name);
		var_name = NULL;
		tkn = tkn->next;
	}
	if (var_name != NULL)
		free(var_name);
	return ((*data)->local_err_state = 0, 1);
}
