/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapichec <mapichec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/10/28 13:00:49 by mapichec         ###   ########.fr       */
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
	if (cmd_args && print == 0)
		free_char_array(cmd_args);
	if ((*data)->env_p)
		free_char_array((*data)->env_p);
	free_env_list((*data)->env_list);
	free_tokens(data, (*tokens));
	free((*data)->end);
	print = (*data)->local_err_state;
	free((*data));
	exit(print);
}

static int	ft_count(char const *s, char c)
{
	int	i;
	int	sn;

	i = 0;
	sn = 0;
	while (s[i] != 0)
	{
		if (s[i] != c)
		{
			sn++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		if (s[i] != '\0')
			i++;
	}
	return (sn);
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

	//cmd_args = ft_calloc(sizeof(char **), 1);
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
	//print_tokens(current);
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_COMMAND)
		{
			current = current->next;
			while (current->type == TOKEN_WHITESPACE)
				current = current->next;
			if (current->type == TOKEN_APPENDICE)
			{
				while(current && !(current->type <= TOKEN_EOF && current->type >= 2))
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
						current = current->next;
						x++;
						continue;
					}
					if (current && !(current->type <= TOKEN_EOF && current->type >= 2))
						current = current->next;
				}
			}
		}
		if (current)
			current = current->next;
	}
	if (tmp && tmp != NULL)
	{
		cmd_args = ft_split(tmp, 32);
		cmd = ft_strdup(cmd_args[0]);
	}
	else
	{
		cmd = NULL;
		cmd_args = NULL;
	}
	if (tmp && !cmd_args)
		cmd_args[0] = ft_strdup(tmp);
	free((*data)->command2);
	// if (tmp2)
	// 	free(tmp2);
	(*data)->tmp6 = NULL;
	if (cmd_args)
	{
		(*data)->tmp6 = tmp;
		(*data)->command2 = cmd;
		(*data)->cmd_args_exit = cmd_args;
		if (manual_cmd(cmd_args, data, tokens))
		{
			free(tmp);
			tmp = NULL;
			free(tmp2);
			tmp2 = NULL;
			free(cmd);
			cmd = NULL;
			manual_helper(data, tkn, cmd_args);
		}
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
	if (cmd && !holder)
	{
		if (cmd_args)
			free_char_array(cmd_args);
		cmd_args = ft_calloc(sizeof(char **), 1);
		holder = ft_strndup(cmd, ft_strlen(cmd));
	}
	if (cmd)
		free(cmd);
	//ft_printf("%s\n", holder);
	//ft_printf("%s\n", cmd_args[0]);
	//ft_printf("%s\n", cmd_args[1]);
	//write(1, cmd_args[0], ft_strlen(cmd_args[0]));
	//ft_printf("%s\n", cmd_args[2]);
	//if (holder)
		//if (execve(tmp, cmd_args, envp))
			//exit(write(2, "fuck execve", 12));
	//ft_printf("non dovrei esserci!\n");
	//free_char_array(cmd_args);
	if (cmd_args)
		execve(holder, cmd_args, envp);
	if (holder)
		free(holder);
	if (tmp2)
		free(tmp2);
	if (tmp)
		free(tmp);
	//free_char_array(cmd_args);
	//write(2, "ciao", 5);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	//free(parent);
	//free((*data)->tmp6);
	exec_exit2(data, tkn, cmd_args, 0);
	return (0);
}

int	handle_heredoc(char *delimiter, t_data **data)
{
	char		*line;

	if ((*data)->fd < 0)
		exit (write(2, "Failed to open heredoc temporary file\n", 39));
	// signal_doc();
	while (1 && g_err_state != 130)
	{
		if (g_err_state == 130)
			return (1);
		line = readline("> ");
		if (line == NULL && write(1, "\n", 1))
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
	if (g_err_state == 130)
		return (1);
	return (0);
}
