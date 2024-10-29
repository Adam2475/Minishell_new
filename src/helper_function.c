/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_function.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 15:01:08 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 17:09:23 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	command_init(t_data *data, t_token *tokens)
{
	if (piper(&tokens) == 0)
		token_parser(&tokens, &data);
	else
		do_pipe(data, tokens);
}

int	init_data(t_data **data, int argc, char **argv, t_token **tokens)
{
	*data = ft_calloc(1, sizeof(t_data));
	if (!argc)
		return (1);
	*argv = NULL;
	g_err_state = 0;
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

int	helper_function2(t_token **current,
	t_token_type type, t_token **tkn)
{
	if (((*current)->type == 9 || (*current)->type == 10)
		&& (*current)->next->type == 12
		&& (*current)->next->next->type == type)
	{
		type = (*current)->type;
		(*tkn) = (*tkn)->next;
		ft_free_null((*current)->value);
		(*current)->next = NULL;
		free((*current));
		(*current) = NULL;
		if ((*tkn)->next && (*tkn)->next->type == type)
		{
			tkn_delone(tkn, (*tkn)->next);
			return (1);
		}
		else
			(*current) = (*tkn)->next;
	}
	return (0);
}

void	free_tokens_helper(t_data **data)
{
	if ((*data)->cmd2)
	{
		free((*data)->cmd2);
		(*data)->cmd2 = NULL;
	}
	if ((*data)->cmd_args)
	{
		free_char_array((*data)->cmd_args);
		(*data)->cmd_args = NULL;
	}
	(*data)->redirect_state_out = -1;
	(*data)->redirect_state_in = -1;
}

char	*trim_whitespace(char *str)
{
	char	*end;

	while (*str == 32)
		str++;
	if (*str == 0)
		return (str);
	end = str + ft_strlen(str) - 1;
	while (end > str && *end == 32)
		end--;
	*(end + 1) = '\0';
	return (str);
}
