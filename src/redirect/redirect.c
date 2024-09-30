/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:04:42 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/30 16:55:33 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Helper function to check if the content of a node is all whitespace
int is_whitespace(const char *str)
{
	while (*str)
	{
		if (!isspace((unsigned char)*str))
			return 0;
		str++;
	}
	return 1;
}

// Function to remove whitespace nodes
void remove_whitespace_nodes(t_token **head)
{
	t_token	*current = *head;
	t_token	*prev = NULL;

	while (current != NULL)
	{
		if (is_whitespace(current->value))
		{
			if (prev == NULL)
			{
				*head = current->next;
				free(current->value);
				free(current);
				current = *head;
			}
			else
			{
				prev->next = current->next;
				free(current->value);
				free(current);
				current = prev->next;
			}
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

// Function to create a new list node
t_token *new_node(const char *content)
{
	t_token *node = (t_token *)malloc(sizeof(t_token));
	if (!node)
		return NULL;
	node->value = ft_strdup(content);
	node->type = 0;
	node->next = NULL;
	return node;
}

char	*trim_whitespace(char *str)
{
	char *end;

	while (*str == 32) str++;
	if (*str == 0)
		return str;
	end = str + ft_strlen(str) - 1;
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

	//ft_printf("%s\n", command);
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

void	handle_heredoc(char *delimiter, t_data **data, char *tmp)
{
	char		*line;
	int			heredoc_fd;

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
	heredoc_fd = open(tmp, O_RDONLY);
	if (heredoc_fd < 0)
	{
		perror("Failed to reopen heredoc temporary file");
		exit(0);
	}
	// while (1)
	// {
	// 	line = get_next_line2(heredoc_fd);
	// 	if (!line)
	// 		break ;
	// 	// da togliere
	// 	// ft_printf("%s", line);
	// 	free(line);
	// }
	close (heredoc_fd);
	exit(0);
}
