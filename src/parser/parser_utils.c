/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/23 17:17:05 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	parser_case_redo(t_token *current, t_data **data)
{
	current = current->next;
	(*data)->redirect_state = 1;
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current->type == TOKEN_APPENDICE)
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
		(*data)->fd = open(current->value, O_RDONLY);
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
	int			status = 0;
	pid_t		parent;
	static char	*tmp = ".heredoc.txt";

	current = current->next;
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	// if the syntax is correct we execute heredoc inside a new fork
	if (current->type == TOKEN_APPENDICE || current->type == TOKEN_COMMAND)
	{
		(*data)->fd = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0600);
		(*data)->redirect_state = 0;
		parent = fork();
		// mettere return 1 per gestire errore con liberazione memoria
		if (parent < 0)
			exit(0);
		if (!parent)
		{
			handle_heredoc(current->value, data, tmp);
			exit (0);
		}
		else if (parent)
			status = parent_here_doc();
	}
	else
		return (ft_printf("syntax error after heredoc operator!\n"));
	return (0);
}
