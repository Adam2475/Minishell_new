/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexers_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


void cmd_exit(char **args, t_data *data)
{
	int exit_code = 0;

	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			printf("exit: %s: numeric argument required\n", args[1]);
			exit_code = 255;
		}
		else
		{
			exit_code = ft_atoi(args[1]);
		}
		if (args[2])
		{
			printf("exit: too many arguments\n");
			return;
		}
	}
	free_exit(&data, data->tokens);
	exit(exit_code);
}
