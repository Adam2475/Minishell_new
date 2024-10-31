/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:37:00 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 13:44:07 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*new_node(const char *content)
{
	t_token	*node;

	node = (t_token *)malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = ft_strdup(content);
	node->type = 0;
	node->next = NULL;
	return (node);
}

int	exec_exit2(t_data **data, t_token **tokens,
		char **cmd_args, int print)
{
	print = 0;
	if (cmd_args && print == 0)
		free_char_array(cmd_args);
	if ((*data)->env_p)
		free_char_array((*data)->env_p);
	free_env_list((*data)->env_list);
	free_tokens(data, (*tokens));
	free((*data)->end);
	print = (*data)->local_err_state;
	free((*data));
	exit(print);
}

void	space_helper(t_token **head, t_token **current,
	t_token **prev, int flag)
{
	if (flag == 0)
	{
		(*head) = (*current)->next;
		free((*current)->value);
		free((*current));
		(*current) = (*head);
	}
	else
	{
		(*prev)->next = (*current)->next;
		free((*current)->value);
		free((*current));
		(*current) = (*prev)->next;
	}
}

int	ft_count(char const *s, char c)
{
	int	i;
	int	sn;

	i = 0;
	sn = 0;
	while (s[i] != 0)
	{
		if (s[i] != c)
		{
			sn++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		if (s[i] != '\0')
			i++;
	}
	return (sn);
}

void	skip_space_qt(t_token **current)
{
	while ((*current) && ((*current)->type == 11 || (*current)->type == 10
			|| (*current)->type == 9))
		(*current) = (*current)->next;
}
