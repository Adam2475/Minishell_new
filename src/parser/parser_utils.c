/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/08 16:51:53 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	parser_case_redo(t_token *current, t_data **data)
{
	current = current->next;
	(*data)->redirect_state = 1;
	while (current && current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current && current->type == TOKEN_APPENDICE)
		(*data)->fd = open(current->value, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		return (1);
	return (0);
}

int	parser_case_redi(t_token *current, t_data **data)
{
	current = current->next;
	(*data)->redirect_state = 0;
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current->type == TOKEN_APPENDICE)
	{
		(*data)->fd = open(current->value, O_RDONLY);
		if ((*data)->fd < 0)
			return (1);
	}
	else
		return (1);
	return (0);
}

int	parser_case_append(t_token *current, t_data **data)
{
	current = current->next;
	(*data)->redirect_state = 1;
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current->type == TOKEN_APPENDICE)
		(*data)->fd = open(current->value, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		return (1);
	return (0);
}

static	int	parent_here_doc(void)
{
	int	status;

	waitpid(-1, &status, 0);
	return (status);
}

int	parser_case_herdoc(t_token *current, t_data **data)
{
	pid_t		parent;
	char		*tmp;

	tmp = ".heredoc.txt";
	current = current->next;
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current->type == TOKEN_APPENDICE || current->type == TOKEN_COMMAND)
	{
		(*data)->fd = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0600);
		(*data)->redirect_state = 0;
		(*data)->heredoc_flag = 1;
		parent = fork();
		if (parent < 0)
			exit(0);
		if (!parent)
			exit (handle_heredoc(current->value, data));
		else if (parent)
			parent_here_doc();
	}
	else
		return (ft_printf("syntax error after heredoc operator!\n"));
	close((*data)->fd);
	(*data)->fd = open(tmp, O_RDONLY);
	return (0);
}
