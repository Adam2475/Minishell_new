/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/31 15:01:31 by mapichec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	close_helper(t_data **data)
{
	if ((*data)->fd >= 0)
		close((*data)->fd);
	if ((*data)->fd_in >= 0)
		close((*data)->fd_in);
	if ((*data)->fd_out >= 0)
		close((*data)->fd_out);
}

static	int	call_for_command(t_token **tokens, t_data **data,
		t_token **origin)
{
	t_token		*current;
	int			i;

	i = 0;
	current = *origin;
	(*data)->command[i++] = ft_strdup(current->value);
	current = current->next;
	while (current->type == 13 || current->type == 1
		|| current->type == 8 || current->type == 11)
	{
		while (current->type == 11 || current->type == 10 || current->type == 9)
			current = current->next;
		if (current->type == TOKEN_APPENDICE || current->type == 14)
		{
			(*data)->command[i] = ft_strdup(current->value);
			current = current->next;
		}
		else
			break ;
		i++;
	}
	execute_command_single((*data)->command, data, tokens);
	close_helper(data);
	return (0);
}

static	int	parser_init(t_data **data)
{
	size_t	words;

	words = (size_t)token_words((*data)->tokens);
	(*data)->command = (char **)ft_calloc(words + 2, sizeof(char *));
	if (!(*data)->command)
		return (1);
	return (0);
}

static	int	return_checker(t_data **data, t_token *current, t_token **tokens)
{
	if (redirect_parser(data, current, tokens) > 0)
		return (write(2, "syntax error\n", 14),
			(*data)->local_err_state = 2, 1);
	if (g_err_state == 130)
		return (1);
	return (0);
}

int	token_parser(t_token **tokens, t_data **data)
{
	t_token		*current;

	if (parser_init(data) > 0)
		free_exit(data);
	current = *tokens;
	while (current && current->type != TOKEN_EOF)
	{
		if ((*data)->heredoc_flag == 0)
			if (return_checker(data, current, tokens) > 0)
				return (1);
		if ((*data)->skip_flag > 0)
			return (0);
		if (current->type == 12 || current->type == 14)
		{
			if (!call_for_command(tokens, data, &current))
				return (0);
			else
				free_exit(data);
		}
		current = current->next;
	}
	return (close_helper(data), 0);
}
