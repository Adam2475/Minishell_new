/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/25 19:08:21 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token_list	*create_token_list_node(t_token *head)
{
	t_token_list *new_node = (t_token_list *)malloc(sizeof(t_token_list));
	if (!new_node)
		return NULL;
	new_node->head = head;
	new_node->next = NULL;
	return new_node;
}

// t_token_list	*split_tokens_by_pipe(t_token *tokens)
// {
// 	t_token_list	*result = NULL;
// 	t_token			*current = tokens;
// 	t_token			*segment_start = tokens;
// 	t_token			*prev = NULL;

// 	while (current)
// 	{
// 		if (current->type == TOKEN_PIPE)
// 		{
// 			if (prev)
// 				prev->next = NULL;

// 			append_token_list(&result, segment_start);

// 			segment_start = current;
// 		}
// 		prev = current;
// 		current = current->next;
// 	}

// 	if (segment_start)
// 		append_token_list(&result, segment_start);

// 	return (result);
// }

t_token_list *split_tokens_by_pipe(t_token *tokens)
{
    t_token_list *result = NULL;
    t_token *current = tokens;
    t_token *segment_start = tokens;
    t_token *prev = NULL;

    while (current)
	{
        if (current->type == TOKEN_PIPE)
		{
            if (prev)
                prev->next = NULL;
            t_token *segment = copy_token_segment(segment_start, current);
            if (!segment) {
                free_token_list2(result);
                return NULL;
            }

			// Append the new segment to the result list
			append_token_list(&result, segment);
			segment_start = current->next;  // Start a new segment after the pipe
        }
        prev = current;
        current = current->next;
    }

	// Append the last segment
    if (segment_start) {
        t_token *segment = copy_token_segment(segment_start, NULL);
        if (!segment) {
            free_token_list2(result);  // Clean up if allocation failed
            return NULL;
        }
        append_token_list(&result, segment);
    }

    return result;  // Return the list of segments
}


t_token *copy_token_segment(t_token *start, t_token *end)
{
    t_token *copy_head = NULL;
    t_token *copy_tail = NULL;
    t_token *current = start;

    while (current && current != end) {
        t_token *new_token = ft_calloc(sizeof(t_token), 1);  // Allocate new token
        if (!new_token)
		{
            free_token_segment(copy_head);  // Free allocated tokens if failure
            return NULL;
        }

        *new_token = *current;  // Shallow copy of token

        // Deep copy dynamically allocated fields
        if (current->value) {
            new_token->value = ft_strdup(current->value);
            if (!new_token->value)
			{
                free(new_token);
                free_token_segment(copy_head);  // Free allocated tokens if failure
                return NULL;
            }
        }

        // Link the copied token to the list
        new_token->next = NULL;
        if (copy_tail)
            copy_tail->next = new_token;
        else
            copy_head = new_token;

        copy_tail = new_token;
        current = current->next;
    }

    return (copy_head);
}

int	init_execution(t_data **data, int *i, char **command)
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
