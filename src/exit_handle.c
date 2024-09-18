/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handle.c                                         :+:      :+:    :+:*/
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:18:31 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 16:40:38 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// serve salvarsi la posizione del puntatore originale

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
	if ((*data)->command)
	{
		free((*data)->command[3]);
		free_char_array((*data)->command);
	}
	if ((*data)->cmd)
		free((*data)->cmd2);
	if ((*data)->cmd)
		free_char_array((*data)->cmd_args);
	free((*data)->input);
	free(*data);
	exit(1);
}
