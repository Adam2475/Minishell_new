/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/30 19:13:58 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	sig_int(void)
{
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	exit(0);
}

static	void	sig_quit(void)
{
	ft_putstr_fd("  \b\b", STDIN_FILENO);
}

static	void	signal_handler(int sign)
{
	int	pid;
	int	status;

	pid = waitpid(-1, &status, 1);
	if (sign == SIGINT)
	{
		g_err_state = 130;
		if (pid == -1)
			sig_int();
		else
			ft_putstr_fd("\n", 1);
	}
	else if (sign == SIGQUIT)
	{
		if (pid == -1)
			sig_quit();
		else
			ft_putstr_fd("\n", 1);
	}
}

void	signal_doc(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
}