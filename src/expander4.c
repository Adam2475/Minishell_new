/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:45:15 by mapichec          #+#    #+#             */
/*   Updated: 2024/10/31 15:36:00 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	case_err(t_token **current, char *tmp, t_data **data)
{
	free((*current)->value);
	if ((*current) && *tmp == '?')
	{
		(*current)->value = expand_err_state(tmp, data);
		return (0);
	}
	(*current)->value = ft_strndup("", 1);
	return (free(tmp), 0);
}

int	ft_isalpha_len2(char *str)
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
		else
			return (-1);
	}
	return (-1);
}

int	expand_var(t_token **tkn_lst, t_data **data)
{
	t_token	*current;

	current = (*tkn_lst);
	while (current && current->type != TOKEN_EOF)
	{
		if (current && current->type == 8)
			expand_doll(&current, data);
		if (current)
			current = current->next;
	}
	current = (*tkn_lst);
	cmd_in_qt(current);
	current = (*tkn_lst);
	doll_to_cmd(tkn_lst);
	shrink_tkn_in_qt(&current);
	return (0);
}

int	check_quotes(t_token **tokens)
{
	t_token	*current;

	current = (*tokens);
	while (current && (int)current->type != TOKEN_EOF)
	{
		if ((int)current->type == 10)
		{
			current = current->next;
			current = ft_set_zero(current, 0);
			if (!current || current->type == TOKEN_EOF)
				return (write(2, "unclosed quote\n", 16));
		}
		if ((int)current->type == 9)
		{
			current = current->next;
			current = ft_set_zero(current, 1);
			if (!current || current->type == TOKEN_EOF)
				return (write(2, "unclosed quote\n", 16));
		}
		current = current->next;
	}
	return (0);
}
