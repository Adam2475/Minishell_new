/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:18:31 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/30 18:09:26 by adapassa         ###   ########.fr       */
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
	while (array && array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

void	free_tokens(t_data **data, t_token *tokens)
{
	if (tokens)
		free_list(tokens);
	if ((*data)->tokens)
		free_list((*data)->tokens);
	if ((*data)->token_list != NULL)
		free_token_list((*data)->token_list);
	if ((*data)->fd >= 0)
		(*data)->fd = -1;
	if ((*data)->path_from_envp)
		free((*data)->path_from_envp);
	if ((*data)->command)
		free_char_array((*data)->command);
	if ((*data)->my_paths)
		free_char_array((*data)->my_paths);
	if ((*data)->my_line)
		free((*data)->my_line);
	if ((*data)->cmd2)
		free((*data)->cmd2);
	if ((*data)->cmd_args)
		free_char_array((*data)->cmd_args);
	free((*data)->input);
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

void free_token_list2(t_token_list *list)
{
    t_token_list *current = list;
    t_token_list *temp;

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
