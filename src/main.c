/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 17:27:55 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_err_state;

void	add_tokens_to_list(t_token_list *result, t_token **src)
{
	while (*src && (*src)->type != 2 && (*src)->type != 7)
	{
		ft_tokenadd_back(&result->head,
			ft_lstnewtoken((*src)->type, ft_strdup((*src)->value)));
		*src = (*src)->next;
	}
}

static void	split_tkn2(int count, t_token_list **result, t_token *src)
{
	int	i;

	i = 0;
	while (i++ < count && src != NULL)
	{
		add_tokens_to_list((*result), &src);
		if (!src)
			break ;
		if (src && src->type == TOKEN_PIPE)
		{
			src = src->next;
			if (src != NULL)
			{
				(*result)->next = (t_token_list *)ft_calloc(1,
						sizeof(t_token_list));
				(*result) = (*result)->next;
			}
		}
	}
}

void	split_tokens(t_data **data, t_token *src)
{
	t_token_list	*result;
	t_token_list	*result_head;
	int				count;

	count = count_pipes(src) + 1;
	result = (t_token_list *)ft_calloc(1, sizeof(t_token_list));
	result_head = result;
	split_tkn2(count, &result, src);
	result->next = NULL;
	(*data)->token_list = result_head;
}

static	void	init_flags(t_data **data)
{
	(*data)->local_err_state = 0;
	(*data)->fd_in = -1;
	(*data)->fd_out = -1;
	(*data)->redirect_state_out = -1;
	(*data)->redirect_state_in = -1;
	(*data)->my_paths = NULL;
	(*data)->path_from_envp = NULL;
	(*data)->my_line = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;
	t_token		*tokens;

	if (init_data(&data, argc, argv, &tokens) > 0)
		return (1);
	if (!envp)
		return (1);
	gen_list_env(&data, envp);
	set_signal();
	init_flags(&data);
	while (1)
	{
		if (tokens)
		{
			free_tokens(&data, tokens);
			tokens = NULL;
		}
		if (!read_input(data))
			return (ft_printf("exit\n"),
				free_exit(&data), data->local_err_state);
		if (!data->input)
			continue ;
		if ((data->input[0] == '\0' || tokenizer(&data, &tokens)))
			continue ;
		env_parser(&data, envp);
		command_init(data, tokens);
	}
}
