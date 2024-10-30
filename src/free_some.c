/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_some.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:09:13 by mapichec          #+#    #+#             */
/*   Updated: 2024/10/30 15:17:04 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

	tmp = head;
	if (tmp == NULL)
		return ;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
}

void	free_exit(t_data **data)
{
	int	j;

	j = (*data)->local_err_state;
	clear_history();
	if ((*data)->env_list)
		free_env_list((*data)->env_list);
	if ((*data)->input)
		free((*data)->input);
	free(*data);
	exit(j);
}

static	void	free_tokens2(t_data **data)
{
	free_tokens_helper(data);
	(*data)->my_paths = NULL;
	(*data)->path_from_envp = NULL;
	(*data)->my_line = NULL;
	free((*data)->input);
	(*data)->my_paths = NULL;
	(*data)->pipes = 0;
	(*data)->path_from_envp = NULL;
	(*data)->my_line = NULL;
}

void	free_tokens(t_data **data, t_token *tokens)
{
	if (tokens)
	{
		free_list(tokens);
		tokens = NULL;
		(*data)->tokens_ptr = NULL;
	}
	(*data)->skip_flag = 0;
	heredoc_unlink(data);
	if ((*data)->tokens)
		free_list((*data)->tokens);
	if ((*data)->token_list != NULL && (*data)->token_list->head == tokens)
		(*data)->token_list->head = NULL;
	if ((*data)->token_list != NULL)
		free_token_list((*data)->token_list);
	if ((*data)->fd >= 0)
		(*data)->fd = -1;
	if ((*data)->path_from_envp != NULL && *(*data)->path_from_envp != 0)
		free((*data)->path_from_envp);
	if ((*data)->command)
		free_char_array((*data)->command);
	if ((*data)->my_paths && *(*data)->my_paths != NULL)
		free_char_array((*data)->my_paths);
	if ((*data)->my_line && *(*data)->my_line != 0)
		free((*data)->my_line);
	free_tokens2(data);
}
