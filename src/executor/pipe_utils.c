/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:41:39 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/01 10:19:22 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

size_t	calculate_command_length(t_token *head)
{
	size_t	total_length;
	t_token	*current;

	total_length = 0;
	current = head;
	while (current)
	{
		total_length += ft_strlen(current->value);
		if (current->next != NULL)
			total_length += 1;
		current = current->next;
	}
	return (total_length);
}

char	*token_to_command(t_token *head)
{
	size_t	command_length;
	char	*command;
	t_token	*current;

	if (!head)
		return (NULL);
	command_length = calculate_command_length(head);
	command = (char *)malloc(command_length + 1);
	if (!command)
	{
		perror("malloc");
		return (NULL);
	}
	command[0] = '\0';
	current = head;
	while (current)
	{
		ft_strcat(command, current->value);
		if (current->next != NULL)
			ft_strcat(command, " ");
		current = current->next;
	}
	return (command);
}

int	count_pipes(t_token *head)
{
	int			count;
	t_token		*current;

	count = 0;
	current = head;
	while (current != NULL)
	{
		if (current->type == TOKEN_PIPE)
		{
			count++;
		}
		current = current->next;
	}
	return (count);
}

void	close_pipes(int *end, int pipes)
{
	int	i;

	i = 0;
	while (i < (2 * pipes))
	{
		close(end[i]);
		i++;
	}
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
