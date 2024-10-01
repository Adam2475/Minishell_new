/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/01 09:42:28 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	sig_int(void)
{
	rl_on_new_line();
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	sig_quit(void)
{
	rl_on_new_line();
	rl_redisplay();
	ft_putstr_fd("  \b\b", STDOUT_FILENO);
}

static void	signal_handler(int signo)
{
	int	pid;
	int	status;

	pid = waitpid(-1, &status, 1);
	if (signo == SIGINT)
	{
		g_err_state = 130;
		if (pid == -1)
			sig_int();
		else
			ft_putstr_fd("\n", 1);
	}
	else if (signo == SIGQUIT)
	{
		if (pid == -1)
			sig_quit();
		else
			ft_putstr_fd("\n", 1);
	}
}

void	set_signal(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
}

void	print_token_lists(t_token_list *token_lists)
{
	int			list_num;
	t_token		*current_token;

	list_num = 1;
	while (token_lists)
	{
		printf("List %d:\n", list_num++);
		current_token = token_lists->head;
		while (current_token)
		{
			printf("  Value: %s, Type: %d\n", current_token->value,
				current_token->type);
			current_token = current_token->next;
		}
		token_lists = token_lists->next;
	}
}
