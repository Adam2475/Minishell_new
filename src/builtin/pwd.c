/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/27 14:30:43 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pwd_cmd(t_data **data)
{
	char		cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_printf("%s\n", cwd);
		return ((*data)->local_err_state = 0, 1);
	}
	else
	{
		perror("getcwd");
		return ((*data)->local_err_state = 1, 1);
	}
}
