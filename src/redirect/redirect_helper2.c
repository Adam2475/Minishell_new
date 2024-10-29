/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_helper2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:54:55 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/29 16:13:35 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	line_checker(char **line)
{
	if (!(*line))
	{
		free(*line);
		return (1);
	}
	return (0);
}

int	handle_heredoc(char *delimiter, t_data **data)
{
	char		*line;

	if ((*data)->fd < 0)
		exit (write(2, "Failed to open heredoc temporary file\n", 39));
	while (1 && g_err_state != 130)
	{
		if (g_err_state == 130)
			return (1);
		line = readline("> ");
		if (line == NULL && write(1, "\n", 1))
			break ;
		if (line_checker(&line))
			continue ;
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter)))
			break ;
		if (ft_strsearch(line, '$'))
			line = expander_doc(line, data);
		write((*data)->fd, line, ft_strlen(line));
		write((*data)->fd, "\n", 1);
		free(line);
	}
	close((*data)->fd);
	if (g_err_state == 130)
		return (1);
	return (0);
}

static	void	command_extractor_helper(t_token *current, char *tmp2,
	char *tmp)
{
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current->type == TOKEN_APPENDICE || current->type == 1)
	{
		while(current && !(current->type <= TOKEN_EOF && current->type >= 2))
		{
			while (current->type == TOKEN_WHITESPACE)
			{
				current = current->next;
					continue;
			}
			if (current->type == TOKEN_APPENDICE || current->type == 1)
			{
				tmp2 = ft_strjoin(tmp, " ");
				free(tmp);
				tmp = ft_strjoin(tmp2, current->value);
				free(tmp2);
				current = current->next;
				continue;
			}
			if (current && !(current->type <= TOKEN_EOF && current->type >= 2))
				current = current->next;
		}
	}
}

void	command_extractor(t_data **data, t_token *current, char **tmp)
{
	char	*tmp2;
	t_token	*head;

	head = current;
	tmp2 = NULL;
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_COMMAND)
		{
			current = current->next;
			command_extractor_helper(current, tmp2, *tmp);
		}
		if (current)
			current = current->next;
	}
	current = head;
}

void	cleanup_helper(t_data **data, char **tmp,
	char **cmd_args, t_token *tkn)
{
	if ((*data)->holder)
		free((*data)->holder);
	if (*tmp)
		free(*tmp);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exec_exit2(data, &tkn, cmd_args, 0);
}