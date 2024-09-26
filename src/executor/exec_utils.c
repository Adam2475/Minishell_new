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

// t_token_list	*create_token_list_node(t_token *head)
// {
// 	t_token_list *new_node = (t_token_list *)malloc(sizeof(t_token_list));
// 	if (!new_node)
// 		return NULL;
// 	new_node->head = head;
// 	new_node->next = NULL;
// 	return new_node;
// }

// // t_token_list	*split_tokens_by_pipe(t_token *tokens)
// // {
// // 	t_token_list	*result = NULL;
// // 	t_token			*current = tokens;
// // 	t_token			*segment_start = tokens;
// // 	t_token			*prev = NULL;

// // 	while (current)
// // 	{
// // 		if (current->type == TOKEN_PIPE)
// // 		{
// // 			if (prev)
// // 				prev->next = NULL;

// // 			append_token_list(&result, segment_start);

// // 			segment_start = current;
// // 		}
// // 		prev = current;
// // 		current = current->next;
// // 	}

// // 	if (segment_start)
// // 		append_token_list(&result, segment_start);

// // 	return (result);
// // }

// t_token_list *split_tokens_by_pipe(t_token *tokens)
// {
//     t_token_list *result = NULL;
//     t_token *current = tokens;
//     t_token *segment_start = tokens;
//     t_token *prev = NULL;

//     while (current)
// 	{
//         if (current->type == TOKEN_PIPE)
// 		{
//             if (prev)
//                 prev->next = NULL;
//             t_token *segment = copy_token_segment(segment_start, current);
//             if (!segment) {
//                 free_token_list2(result);
//                 return NULL;
//             }

// 			// Append the new segment to the result list
// 			append_token_list(&result, segment);
// 			segment_start = current->next;  // Start a new segment after the pipe
//         }
//         prev = current;
//         current = current->next;
//     }

// 	// Append the last segment
//     if (segment_start) {
//         t_token *segment = copy_token_segment(segment_start, NULL);
//         if (!segment) {
//             free_token_list2(result);  // Clean up if allocation failed
//             return NULL;
//         }
//         append_token_list(&result, segment);
//     }

//     return result;  // Return the list of segments
// }


// t_token *copy_token_segment(t_token *start, t_token *end)
// {
//     t_token *copy_head = NULL;
//     t_token *copy_tail = NULL;
//     t_token *current = start;

//     while (current && current != end) {
//         t_token *new_token = ft_calloc(sizeof(t_token), 1);  // Allocate new token
//         if (!new_token)
// 		{
//             free_token_segment(copy_head);  // Free allocated tokens if failure
//             return NULL;
//         }

//         *new_token = *current;  // Shallow copy of token

//         // Deep copy dynamically allocated fields
//         if (current->value) {
//             new_token->value = ft_strdup(current->value);
//             if (!new_token->value)
// 			{
//                 free(new_token);
//                 free_token_segment(copy_head);  // Free allocated tokens if failure
//                 return NULL;
//             }
//         }

//         // Link the copied token to the list
//         new_token->next = NULL;
//         if (copy_tail)
//             copy_tail->next = new_token;
//         else
//             copy_head = new_token;

//         copy_tail = new_token;
//         current = current->next;
//     }

//     return (copy_head);
// }

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

static void free_token_list3(t_token_list *list)
{
    t_token_list *tmp;

    while (list)
    {
        tmp = list;
        list = list->next;

        t_token *current_token = tmp->head;
        while (current_token)
        {
            t_token *next_token = current_token->next;
            free(current_token->value);  // Free the duplicated string
            free(current_token);         // Free the token structure
            current_token = next_token;
        }

        free(tmp);  // Free the t_token_list node
    }
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

// t_token_list *split_tokens_by_pipe(t_token *token_list)
// {
//     t_token_list *result = NULL;
// 	t_token_list *current_list = NULL;
//     t_token *start = token_list;
// 	t_token *prev = NULL;
// 	t_token_list *new_list;

//     while (token_list)
//     {
//         if (token_list->type == TOKEN_PIPE)
//         {
//             if (start != token_list)
//             {
//                 // Temporarily terminate the current segment
//                 if (prev)
//                     prev->next = NULL;

//                 // Create a new sublist for the current segment
//                 new_list = create_token_list_node(start);
//                 if (!result)
//                     result = new_list;
//                 else
//                     current_list->next = new_list;
//                 current_list = new_list;
// 				//free_token_list3(new_list);
// 				free_token_list3(new_list);
//             }
//             // Move start to the token after the pipe
//             start = token_list->next;
//         }
//         prev = token_list;
//         token_list = token_list->next;
//     }

//     // Add the last segment if any
//     if (start)
//     {
//         t_token_list *new_list = create_token_list_node(start);
//         if (!result)
//             result = new_list;
//         else
//             current_list->next = new_list;
//     }

//     return result;
// }

void free_token(t_token *token)
{
    if (token)
    {
        free(token->value);  // Free the duplicated string
        free(token);         // Free the token structure
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


