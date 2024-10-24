/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:26:54 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/24 14:46:08 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	count_pipes(t_token *head)
{
	int			count;
	t_token		*current;

	count = 0;
	current = head;
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

void	close_pipes(int *end, int pipes)
{
	int	i;

	i = 0;
	while (i < (2 * pipes))
	{
		close(end[i]);
		i++;
	}
}

int	exec_exit3(t_data **data, t_token **tokens, int *end, int print)
{
	int	i;

	if (print == 42)
		write(2, "not a file or directory!\n", 26);
	i = 0;
	print = 0;
	g_err_state = errno;
	if ((*data)->env_p && print == 0)
		free_char_array((*data)->env_p);
	while (i < (*data)->pipes * 2)
	{
		close(end[i]);
		i++;
	}
	close(STDOUT_FILENO);
	free_env_list((*data)->env_list);
	free_tokens(data, *tokens);
	free((*data)->end);
	free((*data));
	exit(g_err_state);
}

int	copy_mtx2_pt2(t_data **data, int i)
{
	t_env_list	*node;
	int			j;

	(*data)->env_p = ft_calloc(sizeof(char *), i + 1);
	if (!(*data)->env_p)
		return (1);
	node = (*data)->env_list;
	j = 0;
	while (node && j < i)
	{
		(*data)->env_p[j] = ft_strndup(node->content, ft_strlen(node->content));
		node = node->next;
		j++;
	}
	return (0);
}

int	heredoc_finder(t_token *current)
{
	int	i;

	i = 0;
	g_err_state = 0;
	while (current != NULL)
	{
		if (current->type == TOKEN_HEREDOC)
			i = 1;
		current = current->next;
	}
	return (i);
}
