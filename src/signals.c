/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 17:19:35 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	sig_int(void)
{
	rl_replace_line("\0", 0);
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	ioctl(STDIN_FILENO, TIOCSTI, NULL);
}

static void	sig_quit(void)
{
	ft_putstr_fd("", STDOUT_FILENO);
}

static void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_err_state = 130;
		sig_int();
	}
	else if (signo == SIGQUIT)
		sig_quit();
}

void	set_signal(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
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
