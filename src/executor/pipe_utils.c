/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:41:39 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/21 18:45:02 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

size_t calculate_command_length(t_token *head)
{
	size_t total_length = 0;
	t_token *current = head;
	while (current)
	{
		total_length += strlen(current->value);
		// Add 1 for the space separator
		if (current->next != NULL)
			total_length += 1;
		current = current->next;
	}
	return total_length;
}

char	*token_to_command(t_token *head)
{
	if (!head)
		return NULL;

	size_t command_length = calculate_command_length(head);
	char *command = (char *)malloc(command_length + 1); // +1 for the null terminator

	if (!command)
	{
		perror("malloc");
		return NULL;
	}

	command[0] = '\0'; // Initialize the command string

	t_token *current = head;
	while (current)
	{
		strcat(command, current->value);
		if (current->next != NULL)
			strcat(command, " "); // Add space between tokens
		current = current->next;
	}

	return command;
}

int count_pipes(t_token* head)
{
	int count = 0;
	t_token* current = head;

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