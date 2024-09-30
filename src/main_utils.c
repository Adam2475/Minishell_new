/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/30 19:08:32 by adapassa         ###   ########.fr       */
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

void	env_parser(t_data **data, char **envp)
{
	if ((*data)->env_list == NULL)
		gen_list_env(data, envp);
	(*data)->my_line = retrieve_line(envp);
	if (!(*data)->my_line)
		exit(write(1, "PATH not found\n", 15));
	(*data)->path_from_envp = ft_substr((*data)->my_line, 5, 500);
	(*data)->my_paths = ft_split((*data)->path_from_envp, ':');
}

char	*retrieve_line(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			return (ft_strndup(envp[i], ft_strlen(envp[i])));
		i++;
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
	while (current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}
