/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/21 18:51:58 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*trim_whitespace(char *str)
{
	char *end;

	while (*str == 32) str++;
	if (*str == 0)
		return str;
	end = str + strlen(str) - 1;
	while (end > str && *end == 32) end--;
	*(end + 1) = '\0';
	return (str);
}

int	execute_command(char *command, t_data **data, char **envp)
{
	char *cmd;
	char **cmd_args;
	char *tmp;
	char *holder;

	cmd_args = ft_split(command, 32);
	cmd = cmd_args[0];
	tmp = NULL;
	holder = find_cmd(cmd, data);

	int i = 1;
	while (cmd_args[i])
	{
		tmp = ft_strjoin_gnl(tmp, trim_whitespace(cmd_args[i]));
		i++;
	}
	execve(holder, cmd_args, envp);
	return(0);
}

int	set_redirection(t_token *tokens, t_data **data)
{
	t_token *current = tokens;

	while (current != NULL)
	{
		if (current->type == TOKEN_REDIRECT_OUT)
		{
			current = current->next;
			(*data)->redirect_state = 1;
			if (current->type == TOKEN_APPENDICE)
				(*data)->fd = open(current->value, O_CREAT | O_RDWR | O_TRUNC, 0644);
			else
				return(ft_printf("syntax error!!\n"));
		}
		else if (current->type == TOKEN_REDIRECT_IN)
		{
			current = current->next;
			(*data)->redirect_state = 0;
			if (current->type == TOKEN_APPENDICE)
			{
				(*data)->fd = open(current->value, O_RDONLY);
				if ((*data)->fd < 0)
					exit(ft_printf("input file not found!\n"));
				else
					return(ft_printf("syntax error!!\n"));
			}
		}
		else if (current->type == TOKEN_APPEND)
		{
			current = current->next;
			(*data)->redirect_state = 1;
			if (current->type == TOKEN_APPENDICE)
			{
				ft_printf("setting up the append!\n");
				(*data)->fd = open(current->value, O_WRONLY | O_APPEND | O_CREAT, 0644);
			}
			else
				return(ft_printf("syntax error!!\n"));
		}
		else if (current->type == TOKEN_HEREDOC)
		{
			current = current->next;
			if (current->type == TOKEN_APPENDICE)
				handle_heredoc(current->value, data);
			else
				return(ft_printf("syntax error!!\n"));
		}
		current = current->next;
	}
	return (0);
}

void	handle_heredoc(char *delimiter, t_data **data)
{
	char		*line;
	int			heredoc_fd;
	char		*tempfile;

	tempfile = "/tmp/minishell_heredoc.tmp";
	heredoc_fd = open(tempfile, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (heredoc_fd < 0)
		return (perror("Failed to open heredoc temporary file"));
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		write(heredoc_fd, line, strlen(line));
		write(heredoc_fd, "\n", 1);
		free(line);
	}
	close(heredoc_fd);
	heredoc_fd = open(tempfile, O_RDONLY);
	if (heredoc_fd < 0)
	{
		perror("Failed to reopen heredoc temporary file");
		return ;
	}
	(*data)->fd = heredoc_fd;
	if (dup2((*data)->fd, STDIN_FILENO) < 0)
	{
		perror("heredoc redirection error");
		exit(1);
	}
}
