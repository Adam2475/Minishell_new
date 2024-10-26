/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/26 13:31:22 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	while (dest[i] != '\0')
		i++;
	j = 0;
	while (src[j] != '\0')
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

int	env_parser(t_data **data, char **envp)
{
	if (!(*data)->env_list)
		gen_list_env(data, envp);
	(*data)->my_line = retrieve_line((*data)->env_list);
	if (!(*data)->my_line)
	{
		(*data)->path_from_envp = NULL;
		(*data)->my_paths = NULL;
		(*data)->merdoso = 1;
		return (1);
	}
	(*data)->path_from_envp = ft_substr((*data)->my_line, 5, 500);
	(*data)->my_paths = ft_split((*data)->path_from_envp, ':');
	return (0);
}

char	*retrieve_line(t_env_list *envp)
{
	t_env_list	*node;

	node = envp;
	while (node != NULL)
	{
		if (!ft_strncmp(node->var, "PATH=", 5))
			return (ft_strjoin(node->var, node->value));
		node = node->next;
	}
	return (NULL);
}

void	print_tokens(t_token *tokens)
{
	t_token	*temp;

	temp = tokens;
	while (temp)
	{
		ft_printf("Type: %d, Value: %s\n", temp->type, temp->value);
		temp = temp->next;
	}
}

int	piper(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current && current->type != TOKEN_EOF)
	{
		if (current && current->type == TOKEN_PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}
