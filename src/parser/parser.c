/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/10/24 22:49:08 by mapichec         ###   ########.fr       */
=======
/*   Updated: 2024/10/24 16:54:00 by adapassa         ###   ########.fr       */
>>>>>>> 1c252a35d925e6972e81f9e762829b1851e9110c
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

static	int	token_words(t_token *token)
{
	int		i;
	t_token	*tmp;

	tmp = token;
	i = 0;
	while (tmp && tmp->type != TOKEN_EOF)
	{
		if (tmp->type == TOKEN_WHITESPACE)
			;
		else
			i++;
		tmp = tmp->next;
	}
	return (i + 1);
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

int	token_parser(t_token **tokens, t_data **data)
{
	t_token		*current;

	if (parser_init(data) > 0)
		free_exit(data);
	current = *tokens;
	while (current && current->type != TOKEN_EOF)
	{
		if ((*data)->heredoc_flag == 0)
		{
			if (redirect_parser(data, current, tokens) > 0)
				return (perror(""), 1);
			if (g_err_state == 130)
				return (0);
		}
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
