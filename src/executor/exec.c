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

static	int	child_process(char **cmd_args, t_data **data, char **envp)
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
			exit(ft_printf("fuck off nigga, bad command!\n"));
	}
	else
	{
		g_err_state = 127;
		exit(g_err_state);
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

	init_execution(data, &i);
	(*data)->tmp9 = ft_strjoin(command[0], " ");
	if (manual_cmd(command, data, tokens))
		return (free((*data)->tmp9));
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
		child_process(command, data, envp);
	else
		g_err_state = parent_process();
	return ;
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
