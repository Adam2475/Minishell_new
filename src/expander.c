/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/15 15:01:12 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static	int	case_err(t_token **current, char *tmp)
{
	free((*current)->value);
	if (*tmp == '?')
	{
		(*current)->value = expand_err_state(tmp);
		return (0);
	}
	(*current)->value = ft_strndup("", 1);
	return (free(tmp), 0);
}

static	int	ft_isalpha_len2(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if ((str[i] <= 122 && str[i] >= 97) || (str[i] <= 90 && str[i] >= 65))
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

int	expand_doll(t_token **current, t_data **data)
{
	t_env_list	*node;
	char		*tmp;
	char		*tmp2;
	int			len;

	node = (*data)->env_list;
	len = 0;
	if ((*current)->value)
		tmp = tmp_set((*current)->value);
	if (tmp && ft_strlen(tmp) == 1 && (ft_isalpha(tmp[0]) || tmp[0] == '_'))
	{
		tmp2 = tmp;
		tmp = ft_strjoin(tmp2, "=");
		ft_free_null(tmp2);
	}
	while (node && tmp && ft_strncmp(tmp, node->var, ft_strlen(tmp) - 1) != 0)
	{
		if (node->next)
			node = node->next;
		else if (!node->next)
		{
			node = node->next;
			break ;
		}
	}
	if (!node || *tmp == '?')
		return (case_err(current, tmp));
	ft_free_null(tmp);
	tmp = ft_strtrim2((*current)->value, "$");
	(*current)->value = ft_strdup(tmp);
	ft_free_null(tmp);
	len = ft_isalpha_len2((*current)->value);
	if (len > 0)
	{
		tmp = ft_strndup((*current)->value + len, ft_strlen((*current)->value) - len);
		ft_free_null((*current)->value);
		(*current)->value = ft_strjoin(node->value, tmp);
		return (ft_free_null(tmp), 0);
	}
	else
		ft_free_null((*current)->value);
	return ((*current)->value = ft_strndup(node->value,
			ft_strlen(node->value)), 0);
}

static	void	cmd_in_qt(t_token *current)
{
	char	*tmp;

	tmp = NULL;
	while (current && current->type != TOKEN_EOF)
	{
		if (current && current->next
			&& current->type == 12 && current->next->type != 11
			&& current->next->type != 7)
		{
			tmp = current->value;
			if (current && current->next
				&& current->next->type != 9 && current->next->type != 10)
			{
				current->value = ft_strjoin(current->value,
					current->next->value);
				if (tmp != NULL)
					free(tmp);
			}
			tkn_delone(&current, current->next);
			continue ;
		}
		if (current && current->next)
			current = current->next;
	}
}

int	expand_var(t_token **tkn_lst, t_data **data)
{
	t_token	*current;

	current = (*tkn_lst);
	while (current->type != TOKEN_EOF)
	{
		if (current->type == 8)
			expand_doll(&current, data);
		current = current->next;
	}
	current = (*tkn_lst);
	cmd_in_qt(current);
	current = (*tkn_lst);
	return (0);
}

static t_token	*ft_set_zero(t_token *current, int flag)
{
	while ((int)current->type != 7 && flag == 0
		&& (int)current->type != 10)
	{
		current->type = TOKEN_WORD_QT;
		current = current->next;
	}
	while ((int)current->type != 7 && (int)current->type != 9
		&& flag == 1)
	{
		if ((int)current->type != 8)
			current->type = TOKEN_WORD_QT;
		else if ((int)current->type == 8
			&& (int)current->next->type == 13)
			current = current->next;
		current = current->next;
	}
	return (current);
}

int	check_quotes(t_token **tokens)
{
	t_token	*current;

	current = (*tokens);
	while ((int)current->type != TOKEN_EOF)
	{
		if ((int)current->type == 10)
		{
			current = current->next;
			current = ft_set_zero(current, 0);
			if (current->type == TOKEN_EOF)
				return (ft_printf("unclosed quote\n"));
		}
		if ((int)current->type == 9)
		{
			current = current->next;
			current = ft_set_zero(current, 1);
			if (current->type == TOKEN_EOF)
				return (ft_printf("unclosed quote\n"));
		}
		current = current->next;
	}
	return (0);
}
