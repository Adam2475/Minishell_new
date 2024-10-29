/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 13:00:29 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 10:51:30 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	parser_case_redo(t_token *current, t_data **data)
{
	current = current->next;
	while (current && (current->type == 11
			|| current->type == 10 || current->type == 9))
		current = current->next;
	if (current && (current->type == TOKEN_APPENDICE || current->type == 14))
	{
		(*data)->redirect_state_out = 1;
		(*data)->fd_out = open(current->value,
				O_CREAT | O_RDWR | O_TRUNC, 0644);
		if ((*data)->fd_in < 0)
		{
			if (errno != 0)
				perror("");
			return ((*data)->local_err_state = 1, 0);
		}
	}
	else
		return (1);
	return (0);
}

int	parser_case_append(t_token *current, t_data **data)
{
	current = current->next;
	while (current && (current->type == 11
			|| current->type == 10 || current->type == 9))
		current = current->next;
	if (current && (current->type == TOKEN_APPENDICE || current->type == 14))
	{
		(*data)->redirect_state_out = 1;
		(*data)->fd_out = open(current->value,
				O_WRONLY | O_APPEND | O_CREAT, 0644);
		if ((*data)->fd_out < 0)
		{
			if (errno != 0)
				perror("");
			return ((*data)->local_err_state = 1, 0);
		}
	}
	else
		return (1);
	return (0);
}

int	exec_exit(t_data **data, t_token **tokens, int print)
{
	if ((*data)->fd >= 0)
		close((*data)->fd);
	if ((*data)->saved_fd >= 0)
		close((*data)->saved_fd);
	close(STDOUT_FILENO);
	free_env_list((*data)->env_list);
	free_tokens(data, (*tokens));
	free_char_array((*data)->env_p);
	print = (*data)->local_err_state;
	free((*data));
	exit(print);
}

void	remove_whitespace_nodes(t_token **head)
{
	t_token	*current;
	t_token	*prev;

	current = *head;
	prev = NULL;
	while (current != NULL)
	{
		if (is_whitespace(current->value) && current->type != 14)
		{
			if (prev == NULL)
				space_helper(head, &current, &prev, 0);
			else
				space_helper(head, &current, &prev, 1);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

void	command_single_helper(t_data **data)
{
	if ((*data)->redirect_state_out > 0)
	{
		dup2((*data)->saved_fd_out, STDOUT_FILENO);
		close((*data)->saved_fd_out);
	}
	if ((*data)->redirect_state_in > 0)
	{
		dup2((*data)->saved_fd_in, STDIN_FILENO);
		close((*data)->saved_fd_in);
	}
}
