/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 17:07:50 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	check_cmd_free(char *holder, char *tmp)
{
	if (holder)
		ft_free_null(holder);
	if (tmp)
		ft_free_null(tmp);
}

static	int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

static	void	helper3(char *cmd, t_data **data)
{
	if (errno == 2)
	{
		write(2, "command not found: ", 20);
		write(2, cmd, ft_strlen(cmd));
		write(2, "\n", 1);
		(*data)->local_err_state = 127;
	}
	else
	{
		perror("");
		(*data)->local_err_state = 126;
	}
}

static	void	finder_helper(char *cmd, char **holder,
	char *tmp)
{
	if (compare_path(cmd) > 0)
		*holder = ft_strdup(cmd);
	else
		*holder = ft_strjoin(tmp, cmd);
}

char	*find_cmd(char *cmd, t_data **data)
{
	int		i;
	char	*tmp;
	char	*holder;
	char	*tmp2;

	i = 0;
	if (is_directory(cmd))
		return ((*data)->local_err_state = 126,
			write(2, "Is a directory\n", 16), NULL);
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	while ((*data)->merdoso == 0 && (*data)->my_paths[i])
	{
		tmp2 = ft_strdup((*data)->my_paths[i++]);
		tmp = ft_strjoin(tmp2, "/");
		finder_helper(cmd, &holder, tmp);
		free(tmp2);
		holder = trim_quotes(holder);
		if (access(holder, X_OK) == 0)
			return (ft_free_null(tmp), holder);
		check_cmd_free(holder, tmp);
	}
	helper3(cmd, data);
	return (NULL);
}
