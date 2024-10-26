/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:12:13 by adapassa          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/10/24 22:20:28 by mapichec         ###   ########.fr       */
=======
/*   Updated: 2024/10/24 15:02:35 by adapassa         ###   ########.fr       */
>>>>>>> 1c252a35d925e6972e81f9e762829b1851e9110c
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

<<<<<<< HEAD
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
	free((*data)->parent);
	free((*data));
	exit(g_err_state);
}

static	int	child_process_pipe(t_data **data, t_token *tokens,
	t_token **tkn, pid_t *parent)
=======
static	int	child_process_pipe(char **envp, t_data **data,
	t_token_list *current, t_token **tkn, pid_t *parent, int i)
>>>>>>> 1c252a35d925e6972e81f9e762829b1851e9110c
{
	t_token		*new_tokens;
	t_token		*tokens;

	tokens = current->head;
	pipe_helper(data, current, i);
	new_tokens = extract_command_and_appendices(data, tokens);
	(*data)->command2 = token_to_command(new_tokens);
	free_list(new_tokens);
	copy_mtx2(data);
	free(parent);
	execute_command(data, (*data)->env_p, tkn, &tokens);
	free_char_array((*data)->env_p);
	exit (EXIT_FAILURE);
}

static	void	pipe_case_helper(t_data **data, t_token **tokens, pid_t **parent)
{
	*parent = (pid_t *)ft_calloc(sizeof(pid_t), (count_pipes(*tokens) + 2));
	(*data)->in_tmp = 0;
	(*data)->hd_flag = 0;
	pipe_opener(data, (*data)->end);
	(*data)->in_tmp = dup(STDIN_FILENO);
	(*data)->tokens_ptr = (*tokens);
	(*data)->parent = *parent;
}

static	int	check_stop(t_data **data, int *flag)
{
	if (g_err_state == 130 && (*data)->heredoc_flag == 1)
	{
		g_err_state = 0;
		*flag = 1;
		return(1);
	}
	if (*flag == 1)
	{
		g_err_state = 130;
		return (1);
	}
	return (0);
}

static	int	free_pipe_helper(t_data **data, pid_t **parent)
{
	free(*parent);
	free_char_array((*data)->env_p);
	free((*data)->end);
	return (0);
}

int	pipe_case(t_token **tokens, t_data **data,
	char **envp, t_token_list **token_list)
{
	int				i;
	int				flag;
	pid_t			*parent;
	t_token_list	*current;

	init_pipe(data, tokens, &i, &flag);
	current = *token_list;
	pipe_case_helper(data, tokens, &parent);
	while (++i <= (*data)->pipes)
	{
		if (redirect_parser_pipe(data, current->head, tokens))
<<<<<<< HEAD
		{
			perror("");
			exec_exit3(data, tokens, (*data)->end, 0);
		}
		if (g_err_state == 130 && (*data)->heredoc_flag == 1)
		{
			g_err_state = 0;
			flag = 1;
		}
		if (flag == 1)
		{
			g_err_state = 130;
=======
			exec_exit3(data, tokens, (*data)->end, 42);
		if (check_stop(data, &flag) > 0)
>>>>>>> 1c252a35d925e6972e81f9e762829b1851e9110c
			break ;
		parent[i] = fork();
		if (parent[i] == -1)
			exit(ft_printf("fork error!\n"));
		if (parent[i] == 0)
<<<<<<< HEAD
		{
			// free(parent);
			pipe_helper(data, current, i);
			child_process_pipe(data, current->head, tokens, parent);
			//exit(1);
		}
		dup2(STDIN_FILENO, (*data)->in_tmp);
=======
			child_process_pipe(envp, data, current, tokens, parent, i);
>>>>>>> 1c252a35d925e6972e81f9e762829b1851e9110c
		parent_process2(data, i, (*data)->end);
		current = current->next;
	}
	while (i >= 0)
		waitpid(parent[i--], NULL, 0);
	return (free_pipe_helper(data, &parent));
}
