/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 19:21:15 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	exec_exit(t_data **data, t_token **tokens, int print)
{
	print = 0;
	g_err_state = errno;
	free_env_list((*data)->env_list);
	free_tokens(data, (*tokens));
	free((*data));
	exit(g_err_state);
}

static	int	child_process(char **cmd_args, t_data **data,
	char **envp, t_token **tokens)
{
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
	if ((*data)->cmd2 && cmd_args)
	{
		(*data)->cmd2 = trim_quotes((*data)->cmd2);
		if (execve((*data)->cmd2, cmd_args, envp) != 0)
			exec_exit(data, tokens, ft_printf("Bad command (nigga)!"));
	}
	else
	{
		g_err_state = 127;
		exec_exit(data, tokens, 0);
	}
	return (EXIT_SUCCESS);
}

static	int	parent_process(void)
{
	int	status;

	waitpid(-1, &status, 0);
	return (status);
}

void	execute_command_single(char **command, t_data **data,
		char **envp, t_token **tokens)
{
	pid_t	parent;
	char	*holder;
	int		i;

	(*data)->saved_fd = -1;
	init_execution(data, &i);
	(*data)->tmp9 = ft_strjoin(command[0], " ");
	if (manual_cmd(command, data, tokens))
	{
		if ((*data)->saved_fd >= 0)
		{
			//close(STDOUT_FILENO);
			if ((*data)->redirect_state == 1)
				dup2(STDIN_FILENO, STDOUT_FILENO);
			else if ((*data)->redirect_state == 0)
				dup2((*data)->saved_fd, STDIN_FILENO);
			close((*data)->saved_fd);
			//close((*data)->fd);
			//close(STDOUT_FILENO);
		}
		return (free((*data)->tmp9));
	}
	process_command2(data, command);
	holder = NULL;
	while (command[i])
	{
		holder = ft_strjoin_gnl((*data)->tmp9, command[i++]);
		(*data)->tmp9 = holder;
	}
	(*data)->cmd_args = ft_split((*data)->tmp9, 32);
	free((*data)->tmp9);
	parent = fork();
	if (parent < 0)
		free_exit(data);
	if (!parent)
		child_process(command, data, envp, tokens);
	else
		g_err_state = parent_process();
	return ;
}

void	remove_whitespace_nodes(t_token **head)
{
	t_token	*current;
	t_token	*prev;

	current = *head;
	prev = NULL;
	while (current != NULL)
	{
		if (is_whitespace(current->value))
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
