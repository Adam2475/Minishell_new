/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:12:13 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/22 13:36:42 by adapassa         ###   ########.fr       */
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
	t_token *tokens, t_token **tkn, pid_t *parent)
{
	t_token		*new_tokens;

	new_tokens = extract_command_and_appendices(data, tokens);
	(*data)->command2 = token_to_command(new_tokens);
	envp = NULL;
	free_list(new_tokens);
	copy_mtx2(data);
	free(parent);
	execute_command(data, (*data)->env_p, tkn, &tokens);
	free_char_array((*data)->env_p);
	exit (EXIT_FAILURE);
}

int	heredoc_finder(t_token *current)
{
	int	i;

	i = 0;
	g_err_state = 0;
	while (current != NULL)
	{
		if (current->type == TOKEN_HEREDOC)
			i = 1;
		current = current->next;
	}
	return (i);
}

int	pipe_case(t_token **tokens, t_data **data,
	char **envp, t_token_list **token_list)
{
	int				i;
	int				flag;
	pid_t			*parent;
	t_token_list	*current;

	parent = (pid_t *)ft_calloc(sizeof(pid_t), (count_pipes(*tokens) + 2));
	init_pipe(data, tokens, &i);
	current = *token_list;
	(*data)->in_tmp = 0;
	(*data)->hd_flag = 0;
	pipe_opener(data, (*data)->end);
	flag = 0;
	(*data)->in_tmp = dup(STDIN_FILENO);
	while (++i <= (*data)->pipes)
	{
		//remove_whitespace_nodes(&current->head);
		if (redirect_parser_pipe(data, current->head, tokens))
			exec_exit3(data, tokens, (*data)->end,
				write(2, "not a file or directory!\n", 26));
		if (g_err_state == 130 && (*data)->heredoc_flag == 1)
		{
			g_err_state = 0;
			flag = 1;
		}
		if (flag == 1)
		{
			g_err_state = 130;
			break ;
		}
		//wait(NULL);
		parent[i] = fork();
		if (parent[i] == -1)
			exit(ft_printf("fork error!\n"));
		if (parent[i] == 0)
		{
			// free(parent);
			pipe_helper(data, current, i);
			child_process_pipe(envp, data, current->head, tokens, parent);
			//exit(1);
		}
		dup2(STDIN_FILENO, (*data)->in_tmp);
		parent_process2(data, i, (*data)->end);
		(*data)->redirect_state_out = -1;
		(*data)->redirect_state_in = -1;
		current = current->next;
	}
	while (i >= 0)
	{
		waitpid(parent[i--], NULL, 0);
	}
	free(parent);
	return (free_char_array((*data)->env_p), free((*data)->end), 0);
}
