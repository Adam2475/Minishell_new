/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:18:31 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/19 17:50:19 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// serve salvarsi la posizione del puntatore originale

// void	free_env_list(t_env_list *head)
// {
// 	t_env_list	*tmp;

// 	while (head != NULL)
// 	{
// 		tmp = head;
// 		head = head->next;

// 		if (head != NULL)
// 			head->pre = NULL;
		
// 		if (tmp->value)
// 			free(tmp->value);
// 		if (tmp->var)
// 			free(tmp->var);
// 		if (tmp->content)
// 			free(tmp->content);
// 		tmp->pre = NULL;
// 		tmp->var = NULL;
// 		tmp->value = NULL;
// 		free(tmp);
// 		//tmp = NULL;
// 		//head->pre = NULL;
// 	}
// 	free(head);
// }

void	free_list_env(t_env_list **head)
{
	t_env_list	*temp;

	if (head == NULL || *head == NULL)
		return ;
	while (*head != NULL)
	{
		temp = *head;
		if (temp->next)
			*head = (*head)->next;
		else
		{
			free(temp->value);
			free(temp->var);
			free(temp->content);
			free(temp);
			break ;
		}
		free(temp->value);
		free(temp->var);
		free(temp->content);
		free(temp);
	}
	*head = NULL;
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
	if (tokens)
		free_list(tokens);
	if ((*data)->tmp)
		free_list((*data)->tmp);
	int i = 0;
	if ((*data)->command)
		free_char_array((*data)->command);
	if ((*data)->cmd2)
		free((*data)->cmd2);
	if ((*data)->cmd_args)
		free_char_array((*data)->cmd_args);

	//printf("cioa");
	if ((*data)->env_list)
		free_list_env(&(*data)->env_list);
	//free((*data)->env_list);

	free((*data)->input);
	free(*data);
	exit(1);
}
