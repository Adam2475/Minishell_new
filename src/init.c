/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:39:05 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/01 10:36:09 by adapassa         ###   ########.fr       */
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
		free(tmp);
	}
}

void	free_exit(t_data **data)
{
	clear_history();
	if ((*data)->env_list)
		free_env_list((*data)->env_list);
	if ((*data)->input)
		free((*data)->input);
	free(*data);
	exit(1);
}

int	init_data(t_data **data, int argc, char **argv, t_token **tokens)
{
	*data = ft_calloc(1, sizeof(t_data));
	if (!argc)
		return (1);
	*argv = NULL;
	(*tokens) = NULL;
	if (data)
	{
		(*data)->my_line = NULL;
		(*data)->my_paths = NULL;
		(*data)->path_from_envp = NULL;
		(*data)->env_list = NULL;
		(*data)->redirect_state = -1;
		(*data)->input = NULL;
		(*data)->fd = -1;
		(*data)->command = NULL;
	}
	else
		return (1);
	return (0);
}

int	tokenizer(t_data **data, t_token **tokens)
{
	char	*buffer;
	char	*tmp;
	char	*end;

	end = NULL;
	if (!(*data)->input)
		free_exit(data);
	tmp = ft_strndup((*data)->input, ft_strlen((*data)->input));
	if (init_state(data, tokens, tmp) > 0)
	{
		free(tmp);
		free_exit(data);
	}
	buffer = tmp;
	end = buffer;
	recognizer(buffer, tokens, end, data);
	token_reformatting(tokens);
	if (check_quotes(tokens) != 0)
		return (1);
	expand_var(tokens, data);
	(*data)->tokens = copy_token_list(data, *tokens);
	free (tmp);
	return (0);
}
