/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:18:31 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/21 19:51:38 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// serve salvarsi la posizione del puntatore originale

void free_token_list(t_token_list *list)
{
	t_token_list *current;
	t_token_list *next_node;

	current = list;
	while (current)
	{
		if (current->head)
		{
			if (current->head->value)
				free(current->head->value);
			free(current->head);
		}
		next_node = current->next;
		free(current);
		current = next_node;
	}
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

	while (head != NULL)
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
	if ((*data)->env_list)
		free_env_list((*data)->env_list);
	free((*data)->input);
	free(*data);
	exit(1);
}

void	free_tokens(t_data **data, t_token *tokens)
{
	if (tokens)
		free_list(tokens);
	if ((*data)->tokens)
		free_list((*data)->tokens);
	if ((*data)->tmp)
		free_list((*data)->tmp);
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
	if ((*data)->token_list != NULL)
		free_token_list((*data)->token_list);
	free((*data)->input);
}
