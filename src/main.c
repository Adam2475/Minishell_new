/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:28:11 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_err_state;

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;
	t_token		*tokens;
	t_token		*tmp;
	int			flags;

	if (init_data(&data, argc, argv, &tokens) > 0)
		return (1);
	flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	if (!envp)
		return (1);
	gen_list_env(&data, envp);
	set_signal();
	while (1)
	{
		data->input = readline("myprompt$ ");
		if (!data->input)
			free_exit(&data, tokens, tmp);
		tokenizer(&data, &tokens);
		tmp = copy_token_list(tokens);
		data->tokens = copy_token_list(tokens);
		if (piper(&tokens) == 0)
			token_parser(&tokens, &data, envp);
		else
			printf("found a pipe\n");
		free_exit(&data, tokens);
	}
}
