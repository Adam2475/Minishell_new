/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:18:31 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/25 17:54:00 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_token_list(t_data **data)
{
	t_token_list	*current_list;
	t_token_list	*temp_list;
	t_token			*current_token;
	t_token			*temp_token;

	current_list = (*data)->token_list;
	current_token = current_list->head;
	while (current_list)
	{
		while (current_token)
		{
			temp_token = current_token->next;
			free(current_token->value);
			current_token->value = NULL;
			free(current_token);
			current_token = temp_token;
		}
		temp_list = current_list->next;
		free(current_list);
		current_list = temp_list;
	}
	// if ((*data)->tmp != NULL)
	// 	free((*data)->tmp);
	//print_tokens((*data)->tmp);
}

void	free_env_list(t_env_list *head)
{
	t_env_list	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;

		if (head != NULL)
			head->pre = NULL;
		if (tmp->value)
			free(tmp->value);
		if (tmp->var)
			free(tmp->var);
		if (tmp->content)
			free(tmp->content);
		tmp->pre = NULL;
		tmp->var = NULL;
		tmp->value = NULL;
		free(tmp);
	}
	free(head);
}
void	free_list(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->value)
			free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
}

void	free_exit(t_data **data, t_token *tokens)
{
	clear_history();
	if ((*data)->env_list)
		free_env_list((*data)->env_list);
	if ((*data)->input)
		free((*data)->input);
	free(*data);
	exit(1);
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
		free_token_list(data);
	// if ((*data)->tmp != NULL)
	//  	free((*data)->tmp);
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

void free_token_segment(t_token *token_segment)
{
    t_token *current = token_segment;
    t_token *temp;

    while (current)
	{
        temp = current->next;
        if (current->value) {
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