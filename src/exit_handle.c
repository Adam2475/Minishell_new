/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:18:31 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/19 17:15:47 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// serve salvarsi la posizione del puntatore originale

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
// TODO: sistema
void	free_list(t_token *head)
{
	t_token	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		if (tmp->value && tmp->value[0] != '\0')
			free(tmp->value);
		tmp->value = NULL;
		// ft_printf("%p\n")
		free(tmp);
		tmp = NULL;
	}
}

void	free_exit(t_data **data, t_token *tokens)
{
	free_tokens(data, tokens);
	if ((*data)->env_list)
		free_env_list((*data)->env_list);
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
}
