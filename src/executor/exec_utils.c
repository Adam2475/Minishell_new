/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/08 17:52:41 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*trim_quotes(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*trimmed;

	len = ft_strlen(str);
	trimmed = (char *)ft_calloc(sizeof(char), (len + 1));
	if (!trimmed)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i] != '\'' && str[i] != '\"')
		{
			trimmed[j] = str[i];
			j++;
		}
		i++;
	}
	trimmed[j] = '\0';
	free(str);
	return (trimmed);
}

static	void	helper3(char *cmd)
{
	write(2, "not a file or directory: ", 26);
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
	errno = 127;
}

char	*find_cmd(char *cmd, t_data **data)
{
	int		i;
	char	*tmp;
	char	*holder;
	char	*tmp2;

	i = 0;
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	while ((*data)->merdoso == 0 &&  (*data)->my_paths[i])
	{
		tmp2 = ft_strdup((*data)->my_paths[i++]);
		tmp = ft_strjoin(tmp2, "/");
		if (compare_path(cmd) > 0)
			holder = ft_strdup(cmd);
		else
			holder = ft_strjoin(tmp, cmd);
		free(tmp2);
		holder = trim_quotes(holder);
		if (access(holder, X_OK) == 0)
			return (ft_free_null(tmp), holder);
		if (holder)
			ft_free_null(holder);
		if (tmp)
			ft_free_null(tmp);
	}
	helper3(cmd);
	return (NULL);
}

t_token_list	*split_tokens_by_pipe(t_token *token_list)
{
	t_token_list	*result;
	t_token_list	*current_list;
	t_token			*start;
	t_token			*prev;

	result = NULL;
	current_list = NULL;
	start = token_list;
	prev = NULL;
	while (token_list)
	{
		if (token_list->type == TOKEN_PIPE)
		{
			terminate_segment(prev);
			current_list = create_and_link(start, result, current_list);
			free_token_segment(start);
			start = token_list->next;
		}
		prev = token_list;
		token_list = token_list->next;
	}
	if (start)
		current_list = create_and_link(start, result, current_list);
	return (result);
}
