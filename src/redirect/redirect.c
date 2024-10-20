/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/18 17:42:01 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

int	exec_exit2(t_data **data, t_token **tokens,
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

int	execute_command(t_data **data, char **envp, t_token **tkn, t_token **tokens)
{
	char	*cmd;
	char	**cmd_args;
	char	*holder;
	//int		i;
	char	*tmp;
	t_token	*current = *tokens;
	t_token	*head = *tokens;
	char	*tmp2;

	cmd_args = ft_calloc(sizeof(char **), 1);
	tmp = NULL;
	tmp2 = NULL;
	//print_tokens(*tokens);
	//ft_printf("%s\n", (*data)->command2);
	//cmd_args = ft_split((*data)->command2, 32);
	while (current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_COMMAND)
			tmp = ft_strdup(current->value);
		current = current->next;
	}
	current = head;
	int x = 1;
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_COMMAND)
		{
			current = current->next;
			while (current->type == TOKEN_WHITESPACE)
				current = current->next;
			if (current->type == TOKEN_APPENDICE)
			{
				while(current->type == TOKEN_APPENDICE)
				{
					while (current->type == TOKEN_WHITESPACE)
					{
						current = current->next;
						continue;
					}
					if (current->type == TOKEN_APPENDICE)
					{
						tmp2 = ft_strjoin(tmp, " ");
						free(tmp);
						tmp = ft_strjoin(tmp2, current->value);
						// tmp2 = ft_strjoin(tmp, current->value);
						free(tmp2);
						//cmd_args[x] = ft_strdup(current->value);
						//write(2, tmp, ft_strlen(tmp));
					//	write(2, "|", 1);
						current = current->next;
						x++;
						continue;
					}
				}
			}
		}
		current = current->next;
	}
	// if (!tmp)
	// 	tmp2 =
	// if (tmp)
	// 	ft_printf(tmp);
	// if (tmp)
	// 	tmp2 = ft_strjoin_gnl(tmp, "\0");
	//ft_printf(tmp2);
	// if (tmp)
	// 	free(tmp);
	//cmd_args[x + 1] = NULL;
	//ft_printf("%s\n", tmp2);
	// write(2, tmp2, ft_strlen(tmp2));
	// write(2, "|", 1);
	// write(2, "\n", 1);
	// write(2, tmp, ft_strlen(tmp));
	//
	//write(2, tmp, ft_strlen(tmp) + 1);
	//exit(1);
	//exit(1);
	if (tmp2)
	{
		cmd_args = ft_split(tmp, 32);
		cmd = ft_strdup(cmd_args[0]);
	}
	else
	{
		cmd = NULL;
		cmd_args = NULL;
	}
	free((*data)->command2);
	if (tmp2)
		free(tmp2);
	(*data)->tmp6 = NULL;
	if (cmd_args)
	{
		if (manual_cmd(cmd_args, data, tokens))
			manual_helper(data, tkn, cmd_args);
	}
	if (cmd)
		holder = find_cmd(cmd, data);
	else
		holder = NULL;
	//i = 1;
	// while (cmd_args[i])
	// {
	// 	(*data)->tmp6 = ft_strjoin_gnl((*data)->tmp6,
	// 			trim_whitespace(cmd_args[i++]));
	// }
	//printf("%s\n", (*data)->tmp6);
	if (cmd && !holder)
		holder = ft_strndup(cmd, ft_strlen(cmd));
	if (cmd)
		free(cmd);
	//ft_printf("%s\n", holder);
	// ft_printf("%s\n", cmd_args[0]);
	// ft_printf("%s\n", cmd_args[1]);
	//ft_printf("%s\n", cmd_args[2]);
	if (holder)
		if (!execve(holder, cmd_args, envp))
			exit(write(2, "fuck execve", 12));
	//ft_printf("non dovrei esserci!\n");
	if (holder)
		free(holder);
	//free((*data)->tmp6);
	exec_exit2(data, tkn, cmd_args, 0);
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
		if (g_err_state == 130)
			return (0);
		line = readline("> ");
		if (line == NULL)
			break ;
		if (!line)
		{
			free(line);
			continue ;
		}
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter)))
			break ;
		if (ft_strsearch(line, '$'))
			line = expander_doc(line, data);
		write((*data)->fd, line, ft_strlen(line));
		write((*data)->fd, "\n", 1);
		free(line);
	}
	close((*data)->fd);
	return (0);
}
