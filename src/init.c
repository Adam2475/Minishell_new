/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:39:05 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/01 13:09:06 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	init_data(t_data **data, int argc, char **argv, t_token **tokens)
{
	*data = malloc(sizeof(t_data) * 1);
	if (!argc)
		return (1);
	*argv = NULL;
	(*tokens) = NULL;
	g_err_state = 0;
	if (data)
	{
		(*data)->my_line = NULL;
		(*data)->my_paths = NULL;
		(*data)->path_from_envp = NULL;
		(*data)->env_list = NULL;
		(*data)->input = NULL;
	}
	else
		return (1);
	return (0);
}
