/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:39:05 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 11:06:59 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_env_list(t_env_list *head)
{
	t_env_list	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		if (head != NULL)
			head->pre = NULL;
		if (tmp->value)
			free(tmp->value);
		if (tmp->var)
			free(tmp->var);
		if (tmp->content)
			free(tmp->content);
		tmp->pre = NULL;
		tmp->var = NULL;
		tmp->value = NULL;
		free(tmp);
	}
	free(head);
}

void	free_list(t_token *head)
{
	t_token	*tmp;

	tmp = head;
	if (tmp == NULL)
		return ;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
}

void	free_exit(t_data **data)
{
	int	j;

	j = (*data)->local_err_state;
	clear_history();
	if ((*data)->env_list)
		free_env_list((*data)->env_list);
	if ((*data)->input)
		free((*data)->input);
	free(*data);
	exit(j);
}

static	void	join_in_qt_tk(t_token **tkn)
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
	while (current->next && current->next->type != type)
	{
		tmp = current->value;
		current->value = ft_strjoin(current->value, current->next->value);
		free(tmp);
		tkn_delone(&current, current->next);
	}
	return ;
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
	{
		free(tmp);
		free_exit(data);
	}
	buffer = tmp;
	end = buffer;
	if (recognizer(buffer, tokens, end, data) || token_reformatting(tokens, data))
		return (free(tmp), 1);
	command_reformatter(*tokens);
	// print_tokens(*tokens);
	// exit (0);
	join_in_qt_tk(tokens);
	if (check_quotes(tokens) != 0)
		return (free(tmp), 1);
	expand_var(tokens, data);
	(*data)->tokens = copy_token_list(data, *tokens);
	return (free(tmp), 0);
}
