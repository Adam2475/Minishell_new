/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:12:13 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/18 16:16:48 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_exit3(t_data **data, t_token **tokens, int *end, int print)
{
	int	i;

	i = 0;
	print = 0;
	g_err_state = errno;
	if ((*data)->env_p && print == 0)
		free_char_array((*data)->env_p);
	while (i < (*data)->pipes * 2)
	{
		close(end[i]);
		i++;
	}
	close(STDOUT_FILENO);
	free_env_list((*data)->env_list);
	free_tokens(data, *tokens);
	free((*data)->end);
	free((*data));
	exit(g_err_state);
}

static	int	copy_mtx2_pt2(t_data **data, int i)
{
	t_env_list	*node;
	int			j;

	(*data)->env_p = ft_calloc(sizeof(char *), i + 1);
	if (!(*data)->env_p)
		return (1);
	node = (*data)->env_list;
	j = 0;
	while (node && j < i)
	{
		(*data)->env_p[j] = ft_strndup(node->content, ft_strlen(node->content));
		node = node->next;
		j++;
	}
	return (0);
}

static	void	copy_mtx2(t_data **data)
{
	t_env_list	*node;
	int			i;

	i = 0;
	node = (*data)->env_list;
	while (node)
	{
		if (!node->next)
		{
			i++;
			break ;
		}
		else
		{
			i++;
			node = node->next;
		}
	}
	if (copy_mtx2_pt2(data, i))
		perror("");
}

static	int	child_process_pipe(char **envp, t_data **data,
	t_token *tokens, t_token **tkn)
{
	t_token		*new_tokens;

	new_tokens = extract_command_and_appendices(data, tokens);
	(*data)->command2 = token_to_command(new_tokens);
	envp = NULL;
	if (!((*data)->fd < 0) && !envp)
	{
		if ((*data)->redirect_state == 1)
		{
			if (dup2((*data)->fd, STDOUT_FILENO) < 0)
				exit (-1);
		}
		if ((*data)->redirect_state == 0)
		{
			if (dup2((*data)->fd, STDIN_FILENO) < 0)
				exit (-1);
		}
	}
	free_list(new_tokens);
	copy_mtx2(data);
	//write(2, "ciao", 5);
	execute_command(data, (*data)->env_p, tkn, &tokens);
	free_char_array((*data)->env_p);
	exit (EXIT_FAILURE);
}

int	pipe_case(t_token **tokens, t_data **data,
	char **envp, t_token_list **token_list)
{
	int				i;
	pid_t			*parent;
	t_token_list	*current;

	parent = (pid_t *)ft_calloc(sizeof(pid_t), (count_pipes(*tokens) + 2));
	init_pipe(data, tokens, &i);
	current = *token_list;
	pipe_opener(data, (*data)->end);
	while (++i <= (*data)->pipes)
	{
		//remove_whitespace_nodes(&current->head);
		// if (redirect_parser(data, current->head, tokens))
		// 	exec_exit3(data, tokens, (*data)->end,
		// 		write(2, "not a file or directory!\n", 26));
		//wait(NULL);
		parent[i] = fork();
		if (parent[i] == 0)
		{
			free(parent);
			pipe_helper(tokens, data, current, i);
			child_process_pipe(envp, data, current->head, tokens);
			exit(1);
		}
		if (parent)
			parent_process2(data, i, (*data)->end);
		current = current->next;
	}
	while (i >= 0)
		waitpid(parent[i--], NULL, 0);
	free(parent);
	return (free_char_array((*data)->env_p), free((*data)->end), 0);
}
