/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/30 18:10:38 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	init_execution(t_data **data, int *i)
{
	*i = 1;
	(*data)->cmd2 = NULL;
	return (0);
}

char	*find_cmd(char *cmd, t_data **data)
{
	int		i;
	char	*tmp;
	char	*holder;

	i = 0;
	while ((*data)->my_paths[i])
	{
		tmp = ft_strjoin((*data)->my_paths[i], "/");
		holder = ft_strjoin(tmp, cmd);
		if (access(holder, X_OK) == 0)
			return (free(tmp), holder);
		if (holder)
			free(holder);
		if (tmp)
			free(tmp);
		i++;
	}
	write(2, "command not found : ", 20);
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
	return (NULL);
}

t_token_list	*create_token_list_node(t_token *token_head)
{
	t_token_list	*new_node;

	new_node = ft_calloc(sizeof(t_token_list), 1);
	if (!new_node)
		return NULL;
	new_node->head = token_head;
	new_node->next = NULL;
	return (new_node);
}

void free_token(t_token *token)
{
	if (token)
	{
		free(token->value);
		free(token);
	}
}


t_token_list *split_tokens_by_pipe(t_token *token_list)
{
	t_token_list *result = NULL, *current_list = NULL, *new_list = NULL;
	t_token *start = token_list, *prev = NULL;

	while (token_list)
	{
		if (token_list->type == TOKEN_PIPE)
		{
			if (start != token_list)
			{
				// Temporarily terminate the current segment
				if (prev)
					prev->next = NULL;

				// Create a new sublist for the current segment
				new_list = create_token_list_node(start);
				if (!new_list)
				{
					// Handle memory allocation failure if needed
					return result;  // Return what we have so far
				}
				if (!result)
					result = new_list;
				else
					current_list->next = new_list;

				current_list = new_list;

				// Free the tokens that were added to the new list
				t_token *tmp = start;
				while (tmp->next && tmp != token_list)  // Freeing the range that was just copied
				{
					t_token *next_token = tmp->next;
					free_token(tmp); // Free each token
					tmp = next_token;
				}
			}
			// Move start to the token after the pipe
			start = token_list->next;
		}
		prev = token_list;
		token_list = token_list->next;
	}

	// Add the last segment if any
	if (start)
	{
		new_list = create_token_list_node(start);
		if (!new_list)
		{
			// Handle memory allocation failure if needed
			return result;  // Return what we have so far
		}
		if (!result)
			result = new_list;
		else
			current_list->next = new_list;

		// Free the tokens that were added to the new list
		t_token *tmp = start;
		while (tmp)
		{
			t_token *next_token = tmp->next;
			free_token(tmp); // Free each token
			tmp = next_token;
		}
	}

	return result;
}

void print_token_lists(t_token_list *token_lists)
{
	int list_num = 1;
	while (token_lists)
	{
		printf("List %d:\n", list_num++);
		t_token *current_token = token_lists->head;
		while (current_token)
		{
			printf("  Value: %s, Type: %d\n", current_token->value, current_token->type);
			current_token = current_token->next;
		}
		token_lists = token_lists->next;
	}
}
