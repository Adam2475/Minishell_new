/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 19:21:15 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	init_execution(t_data **data, int *i, char **command)
{
	*i = 1;
	(*data)->cmd = NULL;
	(*data)->cmd = find_cmd(command[0], data);
}

void free_char_array(char **array)
{
	int i;

	i = 0;
	if (array == NULL)
		return;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
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
