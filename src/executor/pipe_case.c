/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:12:13 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/08 18:31:05 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	exec_exit3(t_data **data, t_token **tokens, int *end, int print)
{
	int	i;
	i = 0;
	print = 0;
	g_err_state = errno;
	if ((*data)->env_p && print == 0)
		free_char_array((*data)->env_p);
	// close((*data)->fd);
	while (i < (*data)->pipes)
	{
		close(end[i * 2 + 1]);
		i++;
	}
	free_env_list((*data)->env_list);
	free_tokens(data, *tokens);
	free((*data)->end);
	free((*data));
	exit(g_err_state);
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = ft_strndup(value, ft_strlen(value));
	new_token->next = NULL;
	return (new_token);
}

static	void copy_mtx2(t_data **data)
{
	t_env_list	*node;
	int			i;
	int			j;

	i = 0;
	node = (*data)->env_list;
	while (node)
	{
		if (!node->next)
		{
			i++;
			break;
		}
		else
		{
			i++;
			node = node->next;
		}
	}
	(*data)->env_p = ft_calloc(sizeof(char *), i + 1);
	if (!(*data)->env_p)
		return ;
	node = (*data)->env_list;
	j = 0;
	while (node && j < i)
	{
		(*data)->env_p[j] = ft_strndup(node->content, ft_strlen(node->content));
		node = node->next;
		j++;
	}
}

static	int	child_process_pipe(char **envp, t_data **data,
	t_token *tokens, t_token **tkn)
{
	char		*holder;
	t_token		*new_tokens;

	new_tokens = extract_command_and_appendices(data, tokens);
	holder = token_to_command(new_tokens);
	envp = NULL;
	if (!((*data)->fd < 0) && !envp)
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
	free_list(new_tokens);
	copy_mtx2(data);
	execute_command(holder, data, (*data)->env_p, tkn);
	return (free_char_array((*data)->env_p), EXIT_SUCCESS);
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

static	void	init_pipe(t_data **data, t_token **tokens, int *i)
{
	(*data)->prev_fd = 0;
	(*data)->pipes = count_pipes(*tokens);
	*i = -1;
	(*data)->end = ft_calloc(sizeof(int), (*data)->pipes * 2);
}


int	pipe_case(t_token **tokens, t_data **data,
	char **envp, t_token_list **token_list)
{
	int				i;
	pid_t			parent;
	t_token_list	*current;

	init_pipe(data, tokens, &i);
	current = *token_list;
	pipe_opener(data, (*data)->end);
	while (++i <= (*data)->pipes)
	{
		remove_whitespace_nodes(&current->head);
		parent = fork();
		if (parent == 0)
		{
			setup_pipe(i, (*data)->pipes, (*data)->prev_fd, (*data)->end);
			close_pipes((*data)->end, (*data)->pipes);
			if (redirect_parser(data, current->head))
				exec_exit3(data, tokens, (*data)->end, ft_printf("not a file or directory!\n"));
			child_process_pipe(envp, data, current->head, tokens);
		}
		else
			parent_process2(data, i, (*data)->end, parent);
		current = current->next;
	}
	return (free_char_array((*data)->env_p), free((*data)->end), 0);
}
