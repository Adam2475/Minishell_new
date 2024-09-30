/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:12:13 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/30 19:15:00 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = strdup(value);
	new_token->next = NULL;
	return (new_token);
}

void	append_token(t_token **list, t_token *new_token)
{
	t_token	*temp;

	if (!*list)
	{
		*list = new_token;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
}

t_token	*extract_command_and_appendices(t_data **data, t_token *tokens)
{
	t_token		*result;
	t_token		*current;

	result = NULL;
	(*data)->command_found = 0;
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WHITESPACE)
		{
			current = current->next;
			continue ;
		}
		if (current->type == TOKEN_COMMAND)
		{
			(*data)->command_found = 1;
			append_token(&result, create_token(current->type, current->value));
		}
		else if ((*data)->command_found && (current->type == TOKEN_APPENDICE
				|| current->type == TOKEN_OPTION))
			append_token(&result, create_token(current->type, current->value));
		else if ((*data)->command_found)
			break ;
		current = current->next;
	}
	return (result);
}

static	int	child_process_pipe(char **envp, t_data **data, t_token *tokens)
{
	char		*holder;
	t_token		*new_tokens;

	new_tokens = extract_command_and_appendices(data, tokens);
	holder = token_to_command(new_tokens);

	if (!((*data)->fd < 0))
	{
		if ((*data)->redirect_state == 1)
		{
			if (dup2((*data)->fd, STDOUT_FILENO) < 0)
				return (-1);
		}
		if ((*data)->redirect_state == 0)
		{
			if (dup2((*data)->fd, STDIN_FILENO) < 0)
				return (-1);
		}
	}
	execute_command(holder, data, envp);
	return (EXIT_SUCCESS);
}

void	close_pipes(int *end, int pipes)
{
	for (int i = 0; i < 2 * pipes; i++)
		close(end[i]);
}

void	setup_pipe(int i, int pipes, int prev_fd, int *end)
{
	if (i > 0)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (i < pipes)
		dup2(end[i * 2 + 1], STDOUT_FILENO);
}

void	parent_process(int i, int pipes, int *end, int *prev_fd, int parent)
{
	if (i > 0)
		close(*prev_fd);
	if (i < pipes)
	{
		close(end[i * 2 + 1]);
		*prev_fd = end[i * 2];
	}
	waitpid(parent, NULL, 0);
}

void	pipe_case(t_token **tokens, t_data **data, char **envp, t_token_list **token_list)
{
	int	pipes = count_pipes(*tokens);
	int	end[pipes * 2];
	int	i = 0;
	int	prev_fd = 0;
	pid_t	parent;

	t_token_list	*current = *token_list;

	for (int j = 0; j < pipes; j++)
		pipe(end + (j * 2));

	while (i <= pipes)
	{
		remove_whitespace_nodes(&current->head);
		parent = fork();
		if (parent == 0)
		{
			setup_pipe(i, pipes, prev_fd, end);
			close_pipes(end, pipes);
			if (redirect_parser(data, current->head) > 0)
				return ;
			child_process_pipe(envp, data, current->head);
		}
		else
			parent_process(i, pipes, end, &prev_fd, parent);
		current = current->next;
		i++;
	}
}
