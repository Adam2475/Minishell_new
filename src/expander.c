/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/23 13:07:22 by adapassa         ###   ########.fr       */
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
			&& !(current->next->type <= 7 && current->next->type >= 2))
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
	int		flag;

	current = (*tkn);
	flag = 1;
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
static	void	join_in_qt_tk(t_token *tkn, t_token_type type)
{
	t_token			*current;
	char			*tmp;
	// static int		i = 0;

	current = tkn;
	tmp = NULL;
	current = current->next;
	// ft_printf("\n\nprint_tokens %d\n", i++);
	// print_tokens(tkn);
	while (current->next && current->next->type != type)
	{
		tmp = current->value;
		current->value = ft_strjoin(current->value, current->next->value);
		free(tmp);
		tkn_delone(&current, current->next);
	}
	return ;
}

static	int	shrink_tkn_in_qt(t_token **tokens)
{
	t_token	*current;
 
	current = (*tokens);
	// ft_printf("\n\nPRIMO\n");
	// print_tokens(*tokens);
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == 10 || current->type == 9)
			join_in_qt_tk(current, current->type);
		if (current->type == 14 || current->type == 8)
			current = current->next;
		current = current->next;
	}
	//print_tokens(*tokens);
	return (0);
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
	shrink_tkn_in_qt(&current);
	// print_tokens(*tkn_lst);
	// join_in_qt_tk(tkn_lst);
	return (0);
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
