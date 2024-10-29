/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:11:31 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 17:06:03 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	setup_pipe(t_data **data, int i, int prev_fd, int *end)
{
	int	pipes;

	pipes = count_pipes((*data)->tokens);
	if (i > 0 && (*data)->hd_flag == 0 && (*data)->redirect_state_in <= 0)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (i < pipes && (*data)->redirect_state_out <= 0)
		dup2(end[i * 2 + 1], STDOUT_FILENO);
	if ((*data)->redirect_state_out > 0)
	{
		if (dup2((*data)->fd_out, STDOUT_FILENO) < 0)
			redirection_out_case_helper(data, end);
		close((*data)->fd_out);
	}
	setup_helper(data, end);
}

char	*trim_quotes(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*trimmed;

	len = ft_strlen(str);
	trimmed = (char *)ft_calloc(sizeof(char), (len + 1));
	if (!trimmed)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i] != '\'' && str[i] != '\"')
		{
			trimmed[j] = str[i];
			j++;
		}
		i++;
	}
	trimmed[j] = '\0';
	free(str);
	return (trimmed);
}

t_token_list	*split_tokens_by_pipe(t_token *token_list)
{
	t_token_list	*result;
	t_token_list	*current_list;
	t_token			*start;
	t_token			*prev;

	result = NULL;
	current_list = NULL;
	start = token_list;
	prev = NULL;
	while (token_list)
	{
		if (token_list->type == TOKEN_PIPE)
		{
			terminate_segment(prev);
			current_list = create_and_link(start, result, current_list);
			free_token_segment(start);
			start = token_list->next;
		}
		prev = token_list;
		token_list = token_list->next;
	}
	if (start)
		current_list = create_and_link(start, result, current_list);
	return (result);
}

int	token_words(t_token *token)
{
	int		i;
	t_token	*tmp;

	tmp = token;
	i = 0;
	while (tmp && tmp->type != TOKEN_EOF)
	{
		if (tmp->type == TOKEN_WHITESPACE)
			;
		else
			i++;
		tmp = tmp->next;
	}
	return (i + 1);
}

void	token_reformatting_helper(t_token **current)
{
	while ((*current)->type == TOKEN_WHITESPACE)
		*current = (*current)->next;
	if (current && (*current)->type == TOKEN_PIPE)
		*current = token_reformatting_pipe(*current);
	if ((*current)->type == TOKEN_DOLLAR)
		(*current) = (*current)->next;
	if (check_type(*current) > 0 && (*current)->type != 2)
		*current = token_reformatting_special(*current);
	if (current && (*current)->type == TOKEN_WORD)
		*current = token_reformatting_command(*current);
	else if (current && !((*current)->type >= 2 && (*current)->type <= 6))
		*current = (*current)->next;
}
