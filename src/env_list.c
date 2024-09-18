/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:18:31 by mapichec          #+#    #+#             */
/*   Updated: 2024/09/15 16:40:38 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env_list	*lstlast_env(t_env_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		if (lst->next == NULL)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	add_back_env(t_env_list **lst, t_env_list *new)
{
	t_env_list	*c;

	if (lst)
	{
		if (*lst)
		{
			c = lstlast_env(*lst);
			c->next = new;
			new->pre = c;
		}
		else
		{
			*lst = new;
			new->pre = NULL;
		}
	}
	else if (!lst)
	{
		lst = &new;
		new->pre = NULL;
	}
}

void	split_var_env(t_env_list **node)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	(*node)->var = (char *)ft_strndup((*node)->content, ft_strlen_char((*node)->content, '='));
	i = ft_strlen_char((*node)->content, '=');
	while ((*node)->content[i] != '\0')
		(*node)->value[j++] = (*node)->content[i++];
}

/*TODO: problemi malloc(): invalid next size (unsorted) quando si presenta un un tab di prese*/
t_env_list	*new_node_env(char *content)
{
	t_env_list	*new;
	int			len;

	len = 0;
	new = NULL;
	new = (t_env_list *)ft_calloc(sizeof(t_env_list), 1);
	if (!new)
		return (NULL);
	new->next = NULL;
	new->content = ft_strndup(content, ft_strlen(content));
	while (new->content[len] != '=')
		len++;
	new->value = (char *)ft_calloc((ft_strlen(new->content) - (len - 1)), sizeof(char));
	new->var = (char *)ft_calloc(len + 2, sizeof(char));
	split_var_env(&new);
	return (new);
}

void	gen_list_env(t_data **data, char **envp)
{
	int			i;
	t_env_list	*node;
	t_env_list	*head;

	i = 0;
	//node = malloc(sizeof(t_env_list) * 1);
	(*data)->env_list = NULL;
	head = (*data)->env_list;
	while (envp[i] != NULL)
	{
		node = new_node_env(envp[i]);
		add_back_env(&head, node);
		i++;
	}
	(*data)->env_list = head;
}
