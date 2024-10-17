/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/17 17:34:32 by adapassa         ###   ########.fr       */
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
	int		i;
	char	*tmp;
	t_token	*current = *tokens;
	t_token	*head = *tokens;

	cmd_args = ft_calloc(sizeof(char **), 1);
	tmp = NULL;
	print_tokens(*tokens);
	//ft_printf("%s\n", (*data)->command2);
	//cmd_args = ft_split((*data)->command2, 32);
	while (current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_COMMAND)
			cmd_args[0] = ft_strdup(current->value);
		current = current->next;
	}
	current = head;
	int x = 1;
	while (current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_COMMAND)
		{
			while (current->type == TOKEN_WHITESPACE)
				current = current->next;
			if (current->type == TOKEN_APPENDICE)
			{
				while(current->type == TOKEN_APPENDICE
					|| current->type == TOKEN_WHITESPACE)
				{
					if (current->type == TOKEN_WHITESPACE)
					{
						current = current->next;
						continue;
					}
					else if (current->type == TOKEN_APPENDICE)
					{
						cmd_args[x] = ft_strdup(current->value);
						current = current->next;
						x++;
						continue;
					}
				}
			}
		}
		current = current->next;
	}
	free((*data)->command2);
	cmd = cmd_args[0];
	(*data)->tmp6 = NULL;
	if (manual_cmd(cmd_args, data, tokens))
		manual_helper(data, tkn, cmd_args);
	holder = find_cmd(cmd, data);
	i = 1;
	// while (cmd_args[i])
	// {
	// 	(*data)->tmp6 = ft_strjoin_gnl((*data)->tmp6,
	// 			trim_whitespace(cmd_args[i++]));
	// }
	//printf("%s\n", (*data)->tmp6);
	if (!holder)
		holder = ft_strndup(cmd, ft_strlen(cmd));
	ft_printf("%s\n", holder);
	ft_printf("%s\n", cmd_args[0]);
	ft_printf("%s\n", cmd_args[1]);
	//ft_printf("%s\n", cmd_args[2]);
	printf("%d\n", execve(holder, cmd_args, envp));
	ft_printf("non dovrei esserci!\n");
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
