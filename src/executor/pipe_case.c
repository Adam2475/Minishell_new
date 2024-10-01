/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:12:13 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/01 11:01:17 by adapassa         ###   ########.fr       */
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

static	void	pipe_opener(t_data **data, int *end)
{
	int	j;

	j = 0;
	while (j < (*data)->pipes)
	{
		pipe(end + (j * 2));
		j++;
	}
}

static	void	init_pipe(t_data **data, t_token **tokens, int **end, int *i)
{
	(*data)->prev_fd = 0;
	(*data)->pipes = count_pipes(*tokens);
	*i = 0;
	*end = ft_calloc(sizeof(int), (*data)->pipes * 2);
}

void	pipe_case(t_token **tokens, t_data **data,
	char **envp, t_token_list **token_list)
{
	int				*end;
	int				i;
	pid_t			parent;
	t_token_list	*current;

	init_pipe(data, tokens, &end, &i);
	current = *token_list;
	pipe_opener(data, end);
	while (i <= (*data)->pipes)
	{
		remove_whitespace_nodes(&current->head);
		parent = fork();
		if (parent == 0)
		{
			setup_pipe(i, (*data)->pipes, (*data)->prev_fd, end);
			close_pipes(end, (*data)->pipes);
			redirect_parser(data, current->head);
			child_process_pipe(envp, data, current->head);
		}
		else
			parent_process2(data, i, end, parent);
		current = current->next;
		i++;
	}
	free(end);
}
