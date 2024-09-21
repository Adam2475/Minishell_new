/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:39:05 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/19 16:15:59 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	init_data(t_data **data, int argc, char **argv, t_token **tokens)
{
	*data = ft_calloc(1, sizeof(t_data));
	if (!argc)
		return (1);
	*argv = NULL;
	(*tokens) = NULL;
	if (data)
	{
		(*data)->my_line = NULL;
		(*data)->my_paths = NULL;
		(*data)->path_from_envp = NULL;
		(*data)->env_list = NULL;
		(*data)->redirect_state = -1;
		(*data)->input = NULL;
		(*data)->fd = -1;
		(*data)->command = NULL;
	}
	else
		return (1);
	return (0);
}
