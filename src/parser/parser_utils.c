/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/16 14:10:02 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

static	int	parent_here_doc(void)
{
	int	status;

	waitpid(-1, &status, 0);
	return (status);
}

static	void	exit_free_heredoc(t_data **data, t_token **tokens)
{
	free_env_list((*data)->env_list);
	free_list(*tokens);
	free_char_array((*data)->env_p);
	free_list((*data)->tokens);
	free((*data)->my_line);
	free_char_array((*data)->my_paths);
	free((*data)->command);
	free((*data)->path_from_envp);
	free((*data));
	exit(g_err_state);
}

static	void	heredoc_case_init(char *tmp, t_data **data)
{
	(*data)->fd = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0600);
	(*data)->redirect_state = 0;
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
		return (ft_printf("syntax error after heredoc operator!\n"));
	close((*data)->fd);
	(*data)->fd = open(tmp, O_RDONLY);
	return (0);
}


int	parser_case_herdoc_pipe(t_token *current, t_data **data, t_token **tokens)
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
		// if (parent < 0)
		// 	exit(printf("ciao"));
		// if (!parent)
		// {
			handle_heredoc(current->value, data);
			//exit_free_heredoc(data, tokens);
		// }
	}
	else
		return (ft_printf("syntax error after heredoc operator!\n"));
	close((*data)->fd);
	(*data)->fd = open(tmp, O_RDONLY);
	return (0);
}
