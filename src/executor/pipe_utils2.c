/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD
/*   Created: 2024/09/21 18:41:39 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/23 18:07:07 by adapassa         ###   ########.fr       */
=======
/*   Created: 2024/10/24 14:26:54 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/24 14:46:08 by adapassa         ###   ########.fr       */
>>>>>>> 1c252a35d925e6972e81f9e762829b1851e9110c
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

<<<<<<< HEAD
size_t	calculate_command_length(t_token *head)
{
	size_t	total_length;
	t_token	*current;

	total_length = 0;
	current = head;
	while (current)
	{
		total_length += ft_strlen(current->value);
		if (current->next != NULL)
			total_length += 1;
		current = current->next;
	}
	return (total_length);
}

char	*token_to_command(t_token *head)
{
	size_t	command_length;
	char	*command;
	t_token	*current;

	if (!head)
		return (NULL);
	command_length = calculate_command_length(head);
	command = (char *)malloc(command_length + 1);
	if (!command)
	{
		perror("malloc");
		return (NULL);
	}
	command[0] = '\0';
	current = head;
	while (current)
	{
		ft_strcat(command, current->value);
		if (current->next != NULL)
			ft_strcat(command, " ");
		current = current->next;
	}
	return (command);
=======
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
>>>>>>> 1c252a35d925e6972e81f9e762829b1851e9110c
}
