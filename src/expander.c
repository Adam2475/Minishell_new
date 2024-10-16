/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/15 15:22:09 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

static	int	call_doll(t_token **current, int *flag)
{
	if ((*flag) == 1 && (*current)->type == TOKEN_DOLLAR)
	{
		(*current)->type = TOKEN_COMMAND;
		(*flag) = 0;
		(*current) = (*current)->next;
		return (1);
	}
	if ((*flag) == 1 && (*current)->type != TOKEN_DOLLAR)
	{
		(*flag) = 0;
		(*current) = (*current)->next;
		return (1);
	}
	if ((*current)->type == TOKEN_PIPE)
	{
		(*flag) = 1;
		(*current) = (*current)->next;
		return (1);
	}
	return (0);
}

static	void	doll_to_cmd(t_token **tkn)
{
	t_token	*current;
	int		flag = 1;

	current = (*tkn);
	while (current && current->type != TOKEN_EOF)
	{
		while (current->type == 11)
		{
			current = current->next;
			continue ;
		}
		if (call_doll(&current, &flag))
			continue ;
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
	doll_to_cmd(tkn_lst);
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
