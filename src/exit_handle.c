/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:18:31 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/01 08:56:44 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_token_list(t_token_list *list)
{
	t_token_list	*tmp;
	t_token			*current_token;
	t_token			*next_token;

	while (list)
	{
		tmp = list;
		list = list->next;
		current_token = tmp->head;
		while (current_token)
		{
			next_token = current_token->next;
			if (current_token)
				free_token(current_token);
			current_token = next_token;
		}
		free(tmp);
	}
}

void	free_char_array(char **array)
{
	int	i;

	i = 0;
	while (array && array != NULL && array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	if (array)
		free(array);
	array = NULL;
}

void	free_token_segment(t_token *token_segment)
{
	t_token	*current;
	t_token	*temp;

	current = token_segment;
	while (current)
	{
		temp = current->next;
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = temp;
	}
}

void	free_token_list2(t_token_list *list)
{
	t_token_list	*current;
	t_token_list	*temp;

	current = list;
	while (current)
	{
		temp = current->next;
		if (current->head)
		{
			free_token_segment(current->head);
		}
		free(current);
		current = temp;
	}
}
