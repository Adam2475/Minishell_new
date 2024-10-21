/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 13:00:29 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/21 18:44:51 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	parser_case_redo(t_token *current, t_data **data)
{
	current = current->next;
	(*data)->redirect_state_out = 1;
	while (current && current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current && current->type == TOKEN_APPENDICE)
		(*data)->fd_out = open(current->value, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		return (1);
	return (0);
}

int	parser_case_append(t_token *current, t_data **data)
{
	current = current->next;
	(*data)->redirect_state_out = 1;
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current->type == TOKEN_APPENDICE)
		(*data)->fd_out = open(current->value, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		return (1);
	return (0);
}

int	exec_exit(t_data **data, t_token **tokens, int print)
{
	errno = print;
	if (g_err_state == 0 && print != 0)
		g_err_state = errno;
	if ((*data)->fd >= 0)
		close((*data)->fd);
	if ((*data)->saved_fd >= 0)
		close((*data)->saved_fd);
	close(STDOUT_FILENO);
	free_env_list((*data)->env_list);
	free_tokens(data, (*tokens));
	free_char_array((*data)->env_p);
	free((*data));
	exit(g_err_state);
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
	if ((*data)->saved_fd >= 0)
	{
		if ((*data)->redirect_state == 1)
			dup2((*data)->saved_fd, STDOUT_FILENO);
		else if ((*data)->redirect_state == 0)
			dup2((*data)->saved_fd, STDIN_FILENO);
		close((*data)->saved_fd);
	}
}
