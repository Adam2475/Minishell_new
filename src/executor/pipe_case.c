/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:12:13 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/27 19:59:59 by mapichec         ###   ########.fr       */
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

static	int	child_process_pipe(t_data **data, t_token *tokens,
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

int	pipe_case(t_token **tokens, t_data **data,
	char **envp, t_token_list **token_list)
{
	int				i;
	int				flag;
	int				status;
	pid_t			*parent;
	t_token_list	*current;

	parent = (pid_t *)ft_calloc(sizeof(pid_t), (count_pipes(*tokens) + 2));
	status = 0;
	init_pipe(data, tokens, &i);
	current = *token_list;
	(*data)->in_tmp = 0;
	(*data)->hd_flag = 0;
	pipe_opener(data, (*data)->end);
	flag = 0;
	(*data)->in_tmp = dup(STDIN_FILENO);
	(*data)->tokens_ptr = (*tokens);
	(*data)->parent = parent;
	while (++i <= (*data)->pipes)
	{
		//remove_whitespace_nodes(&current->head);
		if (redirect_parser_pipe(data, current->head, tokens))
		{
			write(2, "No such file or directory\n", 27);
			exec_exit3(data, tokens, (*data)->end, 0);
		}
		if (g_err_state == 130 && (*data)->heredoc_flag == 1)
		{
			(*data)->local_err_state = 0;
			g_err_state = 0;
			flag = 1;
		}
		if (flag == 1)
		{
			(*data)->local_err_state = 130;
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
			child_process_pipe(data, current->head, tokens, parent);
			//exit(1);
		}
		//dup2(STDIN_FILENO, (*data)->in_tmp);
		parent_process2(data, i, (*data)->end);
		(*data)->redirect_state_out = -1;
		(*data)->redirect_state_in = -1;
		current = current->next;
	}
	while (i >= 0)
	{
		waitpid(parent[i--], &status, 0);
		if (WEXITSTATUS(status))
			(*data)->local_err_state = status;
		else if (WIFSIGNALED(status))
		{
			if (status == 2)
				(*data)->local_err_state = 130;
			if (status == 131)
				(*data)->local_err_state = 131;
		}
	}
	free(parent);
	return (free_char_array((*data)->env_p), free((*data)->end), (*data)->end = NULL, 0);
}
