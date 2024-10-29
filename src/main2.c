/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:53:57 by mapichec          #+#    #+#             */
/*   Updated: 2024/10/29 13:04:08 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	do_pipe(t_data *data, t_token *tokens, char **envp)
{
	t_token	*tmp;

	tmp = copy_token_list(&data, tokens);
	split_tokens(&data, tmp);
	free_list(data->tmp);
	free_list(tmp);
	pipe_case(&tokens, &data, envp, &data->token_list);
}

int	read_input(t_data *data)
{
	data->token_list = NULL;
	data->tokens = NULL;
	data->merdoso = 0;
	data->input = readline("myprompt$ ");
	if (!data->input)
		return (0);
	if (data->input[0] != '\0' && !is_only_spaces(data->input))
		add_history(data->input);
	return (1);
}

int	command_reformatter(t_token *token_list)
{
	int	command_found;

	command_found = 0;
	while (token_list)
	{
		if (token_list->type == TOKEN_PIPE)
			command_found = 0;
		else if (token_list->type == TOKEN_COMMAND)
		{
			if (command_found)
				token_list->type = TOKEN_APPENDICE;
			else
				command_found = 1;
		}
		token_list = token_list->next;
	}
	return (1);
}

int	is_only_spaces(const char *str)
{
	if (str == NULL)
		return (0);
	while (*str)
	{
		if (!ft_isspace((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}
