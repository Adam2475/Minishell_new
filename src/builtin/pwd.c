/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/07 12:28:34 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pwd_cmd(t_data **data)
{
	char		cwd[PATH_MAX];
	t_token		*tkn;

	tkn = (*data)->tokens->next;
	while (tkn && tkn->type != TOKEN_EOF)
	{
		if (tkn->type == TOKEN_WHITESPACE)
			tkn = tkn->next;
		else
			return (ft_printf("pwd: too many arguments\n"));
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_printf("%s\n", cwd);
		return (g_err_state = 0, 1);
	}
	else
	{
		perror("getcwd");
		return (g_err_state = 1, 1);
	}
}
