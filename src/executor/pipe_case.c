/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:12:13 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 13:58:48 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_exit3(t_data **data, t_token **tokens, int *end, int print)
{
	int	i;

	i = 0;
	print = 0;
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
	(*data)->tokens_ptr = NULL;
	free((*data)->end);
	if ((*data)->parent != NULL)
		free((*data)->parent);
	print = (*data)->local_err_state;
	free((*data));
	exit(print);
}

int	child_process_pipe(t_data **data, t_token *tokens,
	t_token **tkn, pid_t *parent)
{
	t_token		*new_tokens;

	new_tokens = extract_command_and_appendices(data, tokens);
	(*data)->command2 = token_to_command(new_tokens);
	free_list(new_tokens);
	copy_mtx2(data);
	free(parent);
	signal(SIGQUIT, SIG_DFL);
	execute_command(data, (*data)->env_p, tkn, &tokens);
	free_char_array((*data)->env_p);
	exit (EXIT_FAILURE);
}

static	int	pipe_helper2(t_data **data, int *flag)
{
	if (g_err_state == 130 && (*data)->heredoc_flag == 1)
	{
		(*data)->local_err_state = 0;
		*flag = 1;
	}
	if (*flag == 1)
	{
		(*data)->local_err_state = 130;
		return (1);
	}
	return (0);
}

static	void	wait_for_childs(t_data **data, pid_t *parent, int i)
{
	while (i >= 0)
	{
		waitpid(parent[i--], &(*data)->status, 0);
		if (WEXITSTATUS((*data)->status))
		{
			(*data)->local_err_state = (*data)->status;
			if ((*data)->local_err_state < 0 || (*data)->local_err_state >= 255)
				(*data)->local_err_state = (*data)->local_err_state % 255;
		}
		else if (WIFSIGNALED((*data)->status))
		{
			if ((*data)->status == 2)
				(*data)->local_err_state = 130;
			if ((*data)->status == 131)
				(*data)->local_err_state = 131;
		}
		else
			(*data)->local_err_state = (*data)->status;
	}
	free(parent);
}

int	pipe_case(t_token **tokens, t_data **data,
	t_token_list **token_list)
{
	int				flag;
	pid_t			*parent;
	t_token_list	*current;

	parent = (pid_t *)ft_calloc(sizeof(pid_t), (count_pipes(*tokens) + 2));
	init_pipe(data, tokens, parent);
	current = *token_list;
	pipe_opener(data, (*data)->end, &flag);
	while (++(*data)->counter <= (*data)->pipes)
	{
		if (redirect_parser_pipe(data, current->head))
			exec_exit3(data, tokens, (*data)->end, 0);
		if (pipe_helper2(data, &flag) > 0)
			break ;
		parent[(*data)->counter] = fork();
		if (parent[(*data)->counter] == -1)
			exit(write(2, "fork error!\n", 13));
		if (parent[(*data)->counter] == 0)
			pipe_helper(data, current, parent, tokens);
		parent_process2(data, (*data)->counter, (*data)->end);
		current = current->next;
	}
	wait_for_childs(data, parent, (*data)->counter);
	return (free_char_array((*data)->env_p),
		free((*data)->end), (*data)->end = NULL, 0);
}
