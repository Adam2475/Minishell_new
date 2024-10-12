/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/12 18:01:45 by adapassa         ###   ########.fr       */
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

static	void	exit_free_heredoc(t_data **data, t_token **tokens)
{
	// if ((*data)->fd >= 0)
	// 	close((*data)->fd);
	// if ((*data)->saved_fd >= 0)
	// 	close((*data)->saved_fd);
	//close(STDOUT_FILENO);
	free_env_list((*data)->env_list);
	//free_tokens(data, (*tokens));
	free_list(*tokens);
	free_char_array((*data)->env_p);
	free_list((*data)->tokens);
	free((*data)->my_line);
	free_char_array((*data)->my_paths);
	//free_
	free((*data)->command);
	free((*data)->path_from_envp);
	free((*data));
	//exit(g_err_state);
	exit(0);
}

int	parser_case_herdoc(t_token *current, t_data **data, t_token **tokens)
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
		{
			handle_heredoc(current->value, data);
			//exec_exit(data, tokens, 0);
			exit_free_heredoc(data, tokens);
			//exit(0);
		}
		else if (parent)
			parent_here_doc();
	}
	else
		return (ft_printf("syntax error after heredoc operator!\n"));
	close((*data)->fd);
	(*data)->fd = open(tmp, O_RDONLY);
	return (0);
}
