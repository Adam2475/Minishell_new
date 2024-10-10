/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/08 18:27:15 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_whitespace(const char *str)
{
	while (*str)
	{
		if (!isspace((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}

void	space_helper(t_token **head, t_token **current,
	t_token **prev, int flag)
{
	if (flag == 0)
	{
		(*head) = (*current)->next;
		free((*current)->value);
		free((*current));
		(*current) = (*head);
	}
	else
	{
		(*prev)->next = (*current)->next;
		free((*current)->value);
		free((*current));
		(*current) = (*prev)->next;
	}
}

t_token	*new_node(const char *content)
{
	t_token	*node;

	node = (t_token *)malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = ft_strdup(content);
	node->type = 0;
	node->next = NULL;
	return (node);
}

static	int	exec_exit2(t_data **data, t_token **tokens,
		char **cmd_args, int print)
{
	print = 0;
	g_err_state = errno;
	if (cmd_args && print == 0)
		free_char_array(cmd_args);
	if ((*data)->env_p)
		free_char_array((*data)->env_p);
	free_env_list((*data)->env_list);
	free_tokens(data, (*tokens));
	free((*data)->end);
	free((*data));
	exit(g_err_state);
}

int	execute_command(char *command, t_data **data, char **envp, t_token **tkn)
{
	char	*cmd;
	char	**cmd_args;
	char	*holder;
	int		i;

	cmd_args = ft_split(command, 32);
	free(command);
	cmd = cmd_args[0];
	(*data)->tmp6 = NULL;
	if (manual_cmd(cmd_args, data, tkn))
	{
		if ((*data)->saved_fd >= 0)
		{
			if ((*data)->redirect_state == 1)
				dup2(STDIN_FILENO, STDOUT_FILENO);
			else if ((*data)->redirect_state == 0)
				dup2((*data)->saved_fd, STDIN_FILENO);
			close((*data)->saved_fd);
		}
		return (exec_exit2(data, tkn, cmd_args, 0));
	}
	holder = find_cmd(cmd, data);
	i = 1;
	while (cmd_args[i])
	{
		(*data)->tmp6 = ft_strjoin_gnl((*data)->tmp6,
				trim_whitespace(cmd_args[i]));
		i++;
	}
	if (!holder)
		holder = ft_strndup(cmd, ft_strlen(cmd));
	if (execve(holder, cmd_args, envp) != 0)
	{
		free(holder);
		free((*data)->tmp6);
		exec_exit2(data, tkn, cmd_args, 0);
	}
	return (0);
}

int	handle_heredoc(char *delimiter, t_data **data)
{
	char		*line;

	if ((*data)->fd < 0)
		exit (ft_printf("Failed to open heredoc temporary file"));
	signal_doc();
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		if (ft_strsearch(line, '$'))
			line = expander_doc(line, data);
		write((*data)->fd, line, ft_strlen(line));
		write((*data)->fd, "\n", 1);
		free(line);
	}
	close((*data)->fd);
	return (0);
}
