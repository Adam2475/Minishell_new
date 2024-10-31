/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:39:05 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 14:56:58 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static	void	join_in_qt_tk2(t_token **tkn)
{
	t_token			*current;
	t_token_type	type;
	char			*tmp;

	current = (*tkn);
	tmp = NULL;
	type = current->type;
	if (helper_function2(&current, type, tkn) > 0)
		return ;
	while (current && current->type == 11 && current->type == 12)
		current = current->next;
	if (current && current->type != 9 && current->type != 10)
		return ;
	type = current->type;
	current = current->next;
	while (current && current->next && current->next->type != type)
	{
		tmp = current->value;
		current->value = ft_strjoin(current->value, current->next->value);
		free(tmp);
		tkn_delone(&current, current->next);
	}
	return ;
}

int	check_token_type(t_token *head)
{
	t_token	*current;

	current = head;
	while (current)
	{
		if (current->type == 14 || current->type == 12
			|| current->type == 13 || current->type == 8)
			return (0);
		current = current->next;
	}
	write(2, "command not found:\n", 19);
	return (-1);
}

int	tokenizer(t_data **data, t_token **tokens)
{
	char	*buffer;
	char	*tmp;
	char	*end;

	end = NULL;
	if (!(*data)->input)
		free_exit(data);
	if ((*data)->command != NULL)
		(*data)->command = NULL;
	tmp = ft_strndup((*data)->input, ft_strlen((*data)->input));
	if (init_state(data, tokens, tmp) > 0)
		return (free(tmp), free_exit(data), 1);
	buffer = tmp;
	end = buffer;
	if (recognizer(buffer, tokens, end, data)
		|| token_reformatting(tokens, data))
		return (free(tmp), 1);
	command_reformatter(*tokens);
	join_in_qt_tk2(tokens);
	if (check_quotes(tokens) != 0)
		return (free(tmp), 1);
	expand_var(tokens, data);
	(*data)->tokens = copy_token_list(data, *tokens);
	return (free(tmp), 0);
}
