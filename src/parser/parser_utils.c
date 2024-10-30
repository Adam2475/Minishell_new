/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/30 10:58:18 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	int	parent_here_doc(void)
{
	int	status;

	waitpid(-1, &status, 0);
	return (status);
}

static	void	exit_free_heredoc(t_data **data, t_token **tokens)
{
	int	tmp;

	tmp = (*data)->local_err_state;
	free_env_list((*data)->env_list);
	free_list(*tokens);
	free_char_array((*data)->env_p);
	free_list((*data)->tokens);
	free((*data)->my_line);
	free_char_array((*data)->my_paths);
	free((*data)->command);
	free((*data)->path_from_envp);
	free((*data));
	exit(tmp);
}

static	void	heredoc_case_init(char *tmp, t_data **data)
{
	(*data)->fd = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if ((*data)->fd_in < 0)
		(*data)->local_err_state = 1;
	(*data)->redirect_state_in = 1;
	(*data)->heredoc_flag = 1;
}

int	parser_case_herdoc(t_token *current, t_data **data, t_token **tokens)
{
	pid_t		parent;
	char		*tmp;

	tmp = ".heredoc.txt";
	current = current->next;
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current->type == 13 || current->type == 12)
	{
		heredoc_case_init(tmp, data);
		parent = fork();
		if (parent < 0)
			exit(0);
		if (!parent)
		{
			if (handle_heredoc(current->value, data))
				exit_free_heredoc(data, tokens);
			exit_free_heredoc(data, tokens);
		}
		parent_here_doc();
	}
	else
		return (write(2, "syntax error after heredoc operator!\n", 38));
	close((*data)->fd);
	return ((*data)->fd_in = open(tmp, O_RDONLY), 0);
}

int	parser_case_herdoc_pipe(t_token *current, t_data **data)
{
	char		*tmp;

	tmp = ".heredoc.txt";
	current = current->next;
	(*data)->hd_flag = 1;
	while (current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (current->type == 13 || current->type == 12)
	{
		heredoc_case_init(tmp, data);
		if ((*data)->fd_in < 0 && errno != 0)
		{
			perror("");
			return ((*data)->local_err_state = 1, 0);
		}
		handle_heredoc(current->value, data);
	}
	else
		return (write(2, "syntax error after heredoc operator!\n", 38));
	close((*data)->fd);
	(*data)->fd_in = open(tmp, O_RDONLY);
	return (0);
}
