/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:41:39 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/23 18:07:07 by adapassa         ###   ########.fr       */
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
